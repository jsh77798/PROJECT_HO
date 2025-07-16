// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Chaos/ChaosEngineInterface.h"
//#include "Components/SceneComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPWeapon)

// Sets default values
APPWeapon::APPWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
}

void APPWeapon::AddFakeProjectileData(int32 NumerOfFakeProjectiles, float ConeHalfAngleInDegrees)
{
	FHitResult HitResult;
	FVector Start = SkeletalMesh->GetSocketLocation(TEXT("Muzzle")); // 또는 카메라, 총구 위치
	FVector End = Start + (GetActorForwardVector() * 1000.0f); // 1000 단위 앞

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자기 자신은 무시

	for (int32 i = 1; i < NumerOfFakeProjectiles; ++i)
	{
		bool ReturnValue = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility,
			QueryParams
		);

		if (ReturnValue)
		{
			
		}
	}

}

void APPWeapon::TriggerFireAudio(USoundBase* Sound, AActor* Actor)
{

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

