// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
APPWeapon::APPWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
}


void APPWeapon::AddFakeProjectileData()
{

}

void APPWeapon::TriggerFireAudio()
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

