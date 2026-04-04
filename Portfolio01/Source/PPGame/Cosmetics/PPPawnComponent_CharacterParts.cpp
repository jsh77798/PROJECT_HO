// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPPawnComponent_CharacterParts.h"

#include "Components/ChildActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Cosmetics/PPCharacterPartTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "HAL/Platform.h"
#include "Math/Transform.h"
#include "Misc/AssertionMacros.h"
#include "Templates/Casts.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/UObjectBaseUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPPawnComponent_CharacterParts)

class UPhysicsAsset;
class USkeletalMesh;
class UWorld;

//////////////////////////////////////////////////////////////////////

FString FPPAppliedCharacterPartEntry::GetDebugString() const
{
	return FString::Printf(TEXT("(PartClass: %s, Socket: %s, Instance: %s)"), *GetPathNameSafe(Part.PartClass), *Part.SocketName.ToString(), *GetPathNameSafe(SpawnedComponent));
}

//////////////////////////////////////////////////////////////////////

FPPCharacterPartHandle FPPCharacterPartList::AddEntry(FPPCharacterPart NewPart)
{
	FPPCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FPPAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;
	
		if (SpawnActorForEntry(NewEntry))
		{
			OwnerComponent->BroadcastChanged();
		}
	}

	return Result;
}

void FPPCharacterPartList::RemoveEntry(FPPCharacterPartHandle Handle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPPAppliedCharacterPartEntry& Entry = *EntryIt;
		if (Entry.PartHandle == Handle.PartHandle)
		{
			const bool bDestroyedActor = DestroyActorForEntry(Entry);
			EntryIt.RemoveCurrent();

			if (bDestroyedActor)
			{
				OwnerComponent->BroadcastChanged();
			}

			break;
		}
	}
}

void FPPCharacterPartList::ClearAllEntries(bool bBroadcastChangeDelegate)
{
	bool bDestroyedAnyActors = false;
	for (FPPAppliedCharacterPartEntry& Entry : Entries)
	{
		bDestroyedAnyActors |= DestroyActorForEntry(Entry);
	}
	Entries.Reset();
	
	if (bDestroyedAnyActors && bBroadcastChangeDelegate)
	{
		OwnerComponent->BroadcastChanged();
	}
}

FGameplayTagContainer FPPCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	for (const FPPAppliedCharacterPartEntry& Entry : Entries)
	{
		if (Entry.SpawnedComponent != nullptr)
		{
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(/*inout*/ Result);
			}
		}
	}

	return Result;
}

bool FPPCharacterPartList::SpawnActorForEntry(FPPAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActors = false;

	if (!OwnerComponent)
	{
		if (Entry.Part.PartClass != nullptr)
		{
			UWorld* World = OwnerComponent->GetWorld();

			if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
			{
				const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

				UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());

				PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
				PartComponent->SetChildActorClass(Entry.Part.PartClass);
				PartComponent->RegisterComponent();

				if (AActor* SpawnedActor = PartComponent->GetChildActor())
				{
					switch (Entry.Part.CollisionMode)
					{
					case ECharacterCustomizationCollisionMode::UseCollisionFromCharacterPart:
						// Do nothing
						break;

					case ECharacterCustomizationCollisionMode::NoCollision:
						SpawnedActor->SetActorEnableCollision(false);
						break;
					}

					// Set up a direct tick dependency to work around the child actor component not providing one
					if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
					{
						SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
					}
				}

				Entry.SpawnedComponent = PartComponent;
				bCreatedAnyActors = true;
			}
		}
	}

	return bCreatedAnyActors;
}

bool FPPCharacterPartList::DestroyActorForEntry(FPPAppliedCharacterPartEntry& Entry)
{
	bool bDestroyedAnyActors = false;

	if (Entry.SpawnedComponent != nullptr)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
		bDestroyedAnyActors = true;
	}

	return bDestroyedAnyActors;
}

//////////////////////////////////////////////////////////////////////

UPPPawnComponent_CharacterParts::UPPPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CharacterPartList(this)
{
}

FPPCharacterPartHandle UPPPawnComponent_CharacterParts::AddCharacterPart(const FPPCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void UPPPawnComponent_CharacterParts::RemoveCharacterPart(FPPCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}

void UPPPawnComponent_CharacterParts::RemoveAllCharacterParts()
{
	CharacterPartList.ClearAllEntries(/*bBroadcastChangeDelegate=*/ true);
}

void UPPPawnComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();
}

void UPPPawnComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CharacterPartList.ClearAllEntries(/*bBroadcastChangeDelegate=*/ false);

	Super::EndPlay(EndPlayReason);
}

TArray<AActor*> UPPPawnComponent_CharacterParts::GetCharacterPartActors() const
{
	TArray<AActor*> Result;
	Result.Reserve(CharacterPartList.Entries.Num());

	for (const FPPAppliedCharacterPartEntry& Entry : CharacterPartList.Entries)
	{
		if (UChildActorComponent* PartComponent = Entry.SpawnedComponent)
		{
			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				Result.Add(SpawnedActor);
			}
		}
	}

	return Result;
}

USkeletalMeshComponent* UPPPawnComponent_CharacterParts::GetParentMeshComponent() const
{
	if (AActor* OwnerActor = GetOwner())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
			{
				return MeshComponent;
			}
		}
	}

	return nullptr;
}

USceneComponent* UPPPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		return MeshComponent;
	}
	else if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}
	else
	{
		return nullptr;
	}
}

FGameplayTagContainer UPPPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}
	else
	{
		return Result;
	}
}

void UPPPawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	// Check to see if the body type has changed
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		// Determine the mesh to use based on cosmetic part tags
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

		// Apply the desired mesh (this call is a no-op if the mesh hasn't changed)
		MeshComponent->SetSkeletalMesh(DesiredMesh, /*bReinitPose=*/ bReinitPose);

		// Apply the desired physics asset if there's a forced override independent of the one from the mesh
		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, /*bForceReInit=*/ bReinitPose);
		}
	}

	// Let observers know, e.g., if they need to apply team coloring or similar
	OnCharacterPartsChanged.Broadcast(this);
}


