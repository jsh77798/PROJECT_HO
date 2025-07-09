// Fill out your copyright notice in the Description page of Project Settings.


#include "PPAbilitySystemComponent.h"

#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
//#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/PPGameplayAbility.h"
#include "Game/AbilitySystem/PPAbilityTagRelationshipMapping.h"
#include "Game/Animation/PPAnimInstance.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "HAL/PlatformMath.h"
#include "HAL/UnrealMemory.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
//#include "PPGlobalAbilitySystem.h"
#include "LogChannels.h"
#include "Misc/AssertionMacros.h"
//#include "System/PPAssetManager.h"
//#include "System/PPGameData.h"
#include "Templates/Casts.h"
#include "Templates/SharedPointer.h"
#include "Templates/SubclassOf.h"
#include "Trace/Detail/Channel.h"
#include "UObject/NameTypes.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPAbilitySystemComponent)

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

UPPAbilitySystemComponent::UPPAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();

	FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

//void UPPAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	//if (UPPGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UPPGlobalAbilitySystem>(GetWorld()))
//	//{
//	//	GlobalAbilitySystem->UnregisterASC(this);
//	//}
//
//	Super::EndPlay(EndPlayReason);
//}

void UPPAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		// Notify all abilities that a new pawn avatar has been set
		//for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		//{
		//	UPPGameplayAbility* PPAbilityCDO = CastChecked<UPPGameplayAbility>(AbilitySpec.Ability);
		//
		//	if (PPAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
		//	{
		//		TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
		//		for (UGameplayAbility* AbilityInstance : Instances)
		//		{
		//			UPPGameplayAbility* PPAbilityInstance = CastChecked<UPPGameplayAbility>(AbilityInstance);
		//			PPAbilityInstance->OnPawnAvatarSet();
		//		}
		//	}
		//	else
		//	{
		//		PPAbilityCDO->OnPawnAvatarSet();
		//	}
		//}

		// Register with the global system once we actually have a pawn avatar. We wait until this time since some globally-applied effects may require an avatar.
		//if (ULyraGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<ULyraGlobalAbilitySystem>(GetWorld()))
		//{
		//	GlobalAbilitySystem->RegisterASC(this);
		//}

		if (UPPAnimInstance* PPAnimInst = Cast<UPPAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			PPAnimInst->InitializeWithAbilitySystem(this);
		}

		TryActivateAbilitiesOnSpawn();
	}
}

void UPPAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		const UPPGameplayAbility* PPAbilityCDO = CastChecked<UPPGameplayAbility>(AbilitySpec.Ability);
		PPAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
	}
}

//void UPPAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
//{
//	ABILITYLIST_SCOPE_LOCK();
//	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
//	{
//		if (!AbilitySpec.IsActive())
//		{
//			continue;
//		}
//
//		UPPGameplayAbility* PPAbilityCDO = CastChecked<UPPGameplayAbility>(AbilitySpec.Ability);
//
//		if (PPAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
//		{
//			// Cancel all the spawned instances, not the CDO.
//			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
//			for (UGameplayAbility* AbilityInstance : Instances)
//			{
//				UPPGameplayAbility* PPAbilityInstance = CastChecked<UPPGameplayAbility>(AbilityInstance);
//
//				if (ShouldCancelFunc(PPAbilityInstance, AbilitySpec.Handle))
//				{
//					if (PPAbilityInstance->CanBeCanceled())
//					{
//						PPAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), PPAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
//					}
//					else
//					{
//						UE_LOG(LogPPAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *PPAbilityInstance->GetName());
//					}
//				}
//			}
//		}
//		else
//		{
//			// Cancel the non-instanced ability CDO.
//			if (ShouldCancelFunc(PPAbilityCDO, AbilitySpec.Handle))
//			{
//				// Non-instanced abilities can always be canceled.
//				check(PPAbilityCDO->CanBeCanceled());
//				PPAbilityCDO->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), FGameplayAbilityActivationInfo(), bReplicateCancelAbility);
//			}
//		}
//	}
//}
//
//void UPPAbilitySystemComponent::CancelInputActivatedAbilities(bool bReplicateCancelAbility)
//{
//	auto ShouldCancelFunc = [this](const UPPGameplayAbility* PPAbility, FGameplayAbilitySpecHandle Handle)
//		{
//			const EPPAbilityActivationPolicy ActivationPolicy = PPAbility->GetActivationPolicy();
//			return ((ActivationPolicy == EPPAbilityActivationPolicy::OnInputTriggered) || (ActivationPolicy == EPPAbilityActivationPolicy::WhileInputActive));
//		};
//
//	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
//}
//
//void UPPAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
//{
//	Super::AbilitySpecInputPressed(Spec);
//
//	// We don't support UGameplayAbility::bReplicateInputDirectly.
//	// Use replicated events instead so that the WaitInputPress ability task works.
//	if (Spec.IsActive())
//	{
//		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
//		//InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
//	}
//}
//
//void UPPAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
//{
//	Super::AbilitySpecInputReleased(Spec);
//
//	// We don't support UGameplayAbility::bReplicateInputDirectly.
//	// Use replicated events instead so that the WaitInputRelease ability task works.
//	if (Spec.IsActive())
//	{
//		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
//		//InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
//	}
//}
//
void UPPAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UPPAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UPPAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//@TODO: See if we can use FScopedServerAbilityRPCBatcher ScopedRPCBatcher in some of these loops

	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UPPGameplayAbility* PPAbilityCDO = CastChecked<UPPGameplayAbility>(AbilitySpec->Ability);

				if (PPAbilityCDO->GetActivationPolicy() == EPPAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UPPGameplayAbility* PPAbilityCDO = CastChecked<UPPGameplayAbility>(AbilitySpec->Ability);

					if (PPAbilityCDO->GetActivationPolicy() == EPPAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UPPAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

//void UPPAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability)
//{
//	Super::NotifyAbilityActivated(Handle, Ability);
//
//	UPPGameplayAbility* PPAbility = CastChecked<UPPGameplayAbility>(Ability);
//
//	AddAbilityToActivationGroup(PPAbility->GetActivationGroup(), PPAbility);
//}
//
//void UPPAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
//{
//	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);
//
//	if (APawn* Avatar = Cast<APawn>(GetAvatarActor()))
//	{
//		if (!Avatar->IsLocallyControlled() && Ability->IsSupportedForNetworking())
//		{
//			//ClientNotifyAbilityFailed(Ability, FailureReason);
//			return;
//		}
//	}
//
//	HandleAbilityFailed(Ability, FailureReason);
//}
//
//void UPPAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
//{
//	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);
//
//	UPPGameplayAbility* PPAbility = CastChecked<UPPGameplayAbility>(Ability);
//
//	RemoveAbilityFromActivationGroup(PPAbility->GetActivationGroup(), PPAbility);
//}
//
//void UPPAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
//{
//	FGameplayTagContainer ModifiedBlockTags = BlockTags;
//	FGameplayTagContainer ModifiedCancelTags = CancelTags;
//
//	if (TagRelationshipMapping)
//	{
//		// Use the mapping to expand the ability tags into block and cancel tag
//		TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags);
//	}
//
//	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, ModifiedBlockTags, bExecuteCancelTags, ModifiedCancelTags);
//
//	//@TODO: Apply any special logic like blocking input or movement
//}
//
//void UPPAbilitySystemComponent::HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled)
//{
//	Super::HandleChangeAbilityCanBeCanceled(AbilityTags, RequestingAbility, bCanBeCanceled);
//
//	//@TODO: Apply any special logic like blocking input or movement
//}
//
//void UPPAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
//{
//	if (TagRelationshipMapping)
//	{
//		TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
//	}
//}
//
//void UPPAbilitySystemComponent::SetTagRelationshipMapping(UPPAbilityTagRelationshipMapping* NewMapping)
//{
//	TagRelationshipMapping = NewMapping;
//}
//
////void UPPAbilitySystemComponent::ClientNotifyAbilityFailed_Implementation(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
////{
////	HandleAbilityFailed(Ability, FailureReason);
////}
//
//void UPPAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
//{
//	//UE_LOG(LogLyraAbilitySystem, Warning, TEXT("Ability %s failed to activate (tags: %s)"), *GetPathNameSafe(Ability), *FailureReason.ToString());
//
//	if (const UPPGameplayAbility* PPAbility = Cast<const UPPGameplayAbility>(Ability))
//	{
//		PPAbility->OnAbilityFailedToActivate(FailureReason);
//	}
//}

bool UPPAbilitySystemComponent::IsActivationGroupBlocked(EPPAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case EPPAbilityActivationGroup::Independent:
		// Independent abilities are never blocked.
		bBlocked = false;
		break;

	case EPPAbilityActivationGroup::Exclusive_Replaceable:
	case EPPAbilityActivationGroup::Exclusive_Blocking:
		// Exclusive abilities can activate if nothing is blocking.
		bBlocked = (ActivationGroupCounts[(uint8)EPPAbilityActivationGroup::Exclusive_Blocking] > 0);
		break;

	default:
		checkf(false, TEXT("IsActivationGroupBlocked: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	return bBlocked;
}

//void UPPAbilitySystemComponent::AddAbilityToActivationGroup(EPPAbilityActivationGroup Group, UPPGameplayAbility* PPAbility)
//{
//	check(PPAbility);
//	check(ActivationGroupCounts[(uint8)Group] < INT32_MAX);
//
//	ActivationGroupCounts[(uint8)Group]++;
//
//	const bool bReplicateCancelAbility = false;
//
//	switch (Group)
//	{
//	case EPPAbilityActivationGroup::Independent:
//		// Independent abilities do not cancel any other abilities.
//		break;
//
//	case EPPAbilityActivationGroup::Exclusive_Replaceable:
//	case EPPAbilityActivationGroup::Exclusive_Blocking:
//		CancelActivationGroupAbilities(EPPAbilityActivationGroup::Exclusive_Replaceable, PPAbility, bReplicateCancelAbility);
//		break;
//
//	default:
//		checkf(false, TEXT("AddAbilityToActivationGroup: Invalid ActivationGroup [%d]\n"), (uint8)Group);
//		break;
//	}
//
//	const int32 ExclusiveCount = ActivationGroupCounts[(uint8)EPPAbilityActivationGroup::Exclusive_Replaceable] + ActivationGroupCounts[(uint8)EPPAbilityActivationGroup::Exclusive_Blocking];
//	if (!ensure(ExclusiveCount <= 1))
//	{
//		UE_LOG(LogPPAbilitySystem, Error, TEXT("AddAbilityToActivationGroup: Multiple exclusive abilities are running."));
//	}
//}
//
//void UPPAbilitySystemComponent::RemoveAbilityFromActivationGroup(EPPAbilityActivationGroup Group, UPPGameplayAbility* PPAbility)
//{
//	check(PPAbility);
//	check(ActivationGroupCounts[(uint8)Group] > 0);
//
//	ActivationGroupCounts[(uint8)Group]--;
//}
//
//void UPPAbilitySystemComponent::CancelActivationGroupAbilities(EPPAbilityActivationGroup Group, UPPGameplayAbility* IgnorePPAbility, bool bReplicateCancelAbility)
//{
//	auto ShouldCancelFunc = [this, Group, IgnorePPAbility](const UPPGameplayAbility* PPAbility, FGameplayAbilitySpecHandle Handle)
//		{
//			return ((PPAbility->GetActivationGroup() == Group) && (PPAbility != IgnorePPAbility));
//		};
//
//	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
//}
//
//void UPPAbilitySystemComponent::AddDynamicTagGameplayEffect(const FGameplayTag& Tag)
//{
//	const TSubclassOf<UGameplayEffect> DynamicTagGE = UPPAssetManager::GetSubclass(UPPGameData::Get().DynamicTagGameplayEffect);
//	if (!DynamicTagGE)
//	{
//		UE_LOG(LogPPAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to find DynamicTagGameplayEffect [%s]."), *UPPGameData::Get().DynamicTagGameplayEffect.GetAssetName());
//		return;
//	}
//
//	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(DynamicTagGE, 1.0f, MakeEffectContext());
//	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
//
//	if (!Spec)
//	{
//		UE_LOG(LogPPAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to make outgoing spec for [%s]."), *GetNameSafe(DynamicTagGE));
//		return;
//	}
//
//	Spec->DynamicGrantedTags.AddTag(Tag);
//
//	ApplyGameplayEffectSpecToSelf(*Spec);
//}
//
//void UPPAbilitySystemComponent::RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag)
//{
//	const TSubclassOf<UGameplayEffect> DynamicTagGE = UPPAssetManager::GetSubclass(UPPGameData::Get().DynamicTagGameplayEffect);
//	if (!DynamicTagGE)
//	{
//		UE_LOG(LogPPAbilitySystem, Warning, TEXT("RemoveDynamicTagGameplayEffect: Unable to find gameplay effect [%s]."), *UPPGameData::Get().DynamicTagGameplayEffect.GetAssetName());
//		return;
//	}
//
//	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(Tag));
//	Query.EffectDefinition = DynamicTagGE;
//
//	RemoveActiveEffects(Query);
//}

void UPPAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle)
{
	//FGameplayAbilityTargetDataHandle CachedTargetDataHandle;
	OutTargetDataHandle = CachedTargetDataHandle;

	//TSharedPtr<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	//if (ReplicatedData.IsValid())
	//{
	//	OutTargetDataHandle = ReplicatedData->TargetData;
	//}
}

FGameplayAbilityTargetDataHandle UPPAbilitySystemComponent::MakeTargetDataFromHit(const FHitResult& Hit)
{
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.Add(new FGameplayAbilityTargetData_SingleTargetHit(Hit));
	CachedTargetDataHandle = TargetData;
	return TargetData;
}
