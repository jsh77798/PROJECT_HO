// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "AudioParameterControllerInterface.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPWeapon)

// Sets default values
APPWeapon::APPWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
}

void APPWeapon::AddFakeProjectileData(int32 NumerOfFakeProjectiles, float ConeHalfAngleInDegrees)
{
	FHitResult OutHit;

	FVector Start = SkeletalMesh->GetSocketLocation(TEXT("Muzzle")); 

	FVector End = ImpactPositions[0] - Start;
	End = End.GetSafeNormal(0.0001);
	End = UKismetMathLibrary::RandomUnitVectorInConeInRadians(End, ConeHalfAngleInDegrees);
	End *= 1000000.0f;
	End += Start;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자기 자신은 무시

	for (int32 i = 1; i < NumerOfFakeProjectiles; ++i)
	{
		bool ReturnValue = GetWorld()->LineTraceSingleByChannel(
			OutHit,
			Start,
			End,
			ECC_Visibility,
			QueryParams
		);

		if (ReturnValue)
		{
			ImpactSurfaceTypes.Add(UPhysicalMaterial::DetermineSurfaceType(OutHit.PhysMaterial.Get()));
			ImpactPositions.Add(OutHit.Location);
			ImpactNormals.Add(OutHit.Normal);
		}
	}
}

void APPWeapon::TriggerFireAudio(USoundBase* Sound, AActor* Actor)
{
	IAudioParameterControllerInterface* AudioInterface = Cast<IAudioParameterControllerInterface>(AudioComponent);

	if (IsValid(AudioComponent))
	{
		AudioInterface->SetTriggerParameter(TEXT("Fire"));
	}
	else 
	{
		UGameplayStatics::SpawnSoundAttached(
			Sound,
			Actor->FindComponentByClass<USkeletalMeshComponent>(),
			TEXT("hand_r"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false
		);
	}
}

// Called when the game starts or when spawned
void APPWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

