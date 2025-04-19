// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/EventActor.h"
#include "Kismet/GameplayStatics.h"
#include <Global/Portfolio_GlobalCharacter.h>

// Sets default values
AEventActor::AEventActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEventActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//// �̺�Ʈ ////

// ����_����Ʈ���̽�
void AEventActor::Attack_LineTrace(FVector CameraLoc, FVector CameraForward, FName AttackType, FName DamageType)
{

	FVector StartLoc = CameraLoc; // ������ ���� ����.
	FVector EndLoc = CameraLoc + (CameraForward * 5000.0f); // ������ ������ ����. //�߽ɺ� źȯ
	FVector EndLoc_L = EndLoc + FVector(-150.0f, 0.0f, 0.0f); //������ źȯ
	FVector EndLoc_R = EndLoc + FVector(150.0f, 0.0f, 0.0f); //������ źȯ
	FVector EndLoc_F = EndLoc + FVector(0.0f, 0.0f, -150.0f); //��ܺ� źȯ
	FVector EndLoc_B = EndLoc + FVector(0.0f, 0.0f, 150.0f); //�ϴܺ� źȯ

	/*
	FVector Direction = (EndLoc - SphereComponent->GetComponentLocation()).GetSafeNormal();
	FVector Movement = Direction * 3000 * GetWorld()->GetDeltaSeconds();
	SphereComponent->AddWorldOffset(Movement);
	*/

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ������.
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	ECollisionChannel CollisionChannel;
	FCollisionResponseParams ResponseParams;
	if (UCollisionProfile::GetChannelAndResponseParams(FName(TEXT("Monster")), CollisionChannel, ResponseParams))
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(CollisionChannel));
	}
	ObjectTypes.Add(WorldStatic);
	ObjectTypes.Add(WorldDynamic);

	TArray<AActor*> IgnoreActors; // ������ ���͵�.

	FHitResult HitResult; // ��Ʈ ��� �� ���� ����.
	FHitResult HitResult_L;
	FHitResult HitResult_R;
	FHitResult HitResult_F;
	FHitResult HitResult_B;

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors, // ������ ���� ���ٰ��ص� null�� ���� �� ����.
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		// ���� �ؿ� 3���� �⺻ ������ ������. �ٲٷ��� ������ ��.
		//, FLinearColor::Red
		//, FLinearColor::Green
		//, 5.0f
	);

	// ���� ����Ʈ���̽�
	bool Result_L = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc_L,
		ObjectTypes,
		false,
		IgnoreActors, // ������ ���� ���ٰ��ص� null�� ���� �� ����.
		EDrawDebugTrace::ForDuration,
		HitResult_L,
		true
	);

	// ���� ����Ʈ���̽�
	bool Result_R = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc_R,
		ObjectTypes,
		false,
		IgnoreActors, // ������ ���� ���ٰ��ص� null�� ���� �� ����.
		EDrawDebugTrace::ForDuration,
		HitResult_R,
		true
	);

	// ����� ����Ʈ���̽�
	bool Result_F = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc_F,
		ObjectTypes,
		false,
		IgnoreActors, // ������ ���� ���ٰ��ص� null�� ���� �� ����.
		EDrawDebugTrace::ForDuration,
		HitResult_F,
		true
	);

	// �ϴ��� ����Ʈ���̽�
	bool Result_B = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc_B,
		ObjectTypes,
		false,
		IgnoreActors, // ������ ���� ���ٰ��ص� null�� ���� �� ����.
		EDrawDebugTrace::ForDuration,
		HitResult_B,
		true
	);

	if (Result_L == true || Result_R == true || Result_F == true || Result_B == true)
	{
		UPrimitiveComponent* HitComponent_L = HitResult_L.GetComponent();
		UPrimitiveComponent* HitComponent_R = HitResult_R.GetComponent();
		UPrimitiveComponent* HitComponent_F = HitResult_F.GetComponent();
		UPrimitiveComponent* HitComponent_B = HitResult_B.GetComponent();

		if (HitComponent_L)
		{
			OtherDamage(HitResult_L);
		}

		if (HitComponent_R)
		{
			OtherDamage(HitResult_R);
		}

		if (HitComponent_F)
		{
			OtherDamage(HitResult_F);
		}

		if (HitComponent_B)
		{
			OtherDamage(HitResult_B);
		}
	}

	if (Result == true)
	{
		float Damage = 100;
		FVector ImpactPoint = HitResult.ImpactPoint;
		HitResult.GetActor();
		// HitResult���� �ʿ��� ������ ����ϸ� ��.

		//float Distance = (HitResult.ImpactPoint - GetActorLocation()).Size();
		//float Impact = (Hit.ImpactPoint).Size();
		// Calculate damage based on distance.
		//float Damage = FMath::Lerp(1.0f, MaxDamageDistance, Distance) * DamagePerUnit;

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent)
		{
			FName BoneName = HitResult.BoneName; // �ٵ� �ݸ����� �� �̸��� �����ɴϴ�.
			// ���� BoneName�� ����Ͽ� �ٵ� �ݸ����� ���õ� �۾��� ������ �� �ֽ��ϴ�.

			// Apply damage to the hit actor (assuming it's an ACharacter).
			APortfolio_GlobalCharacter* HitCharacter = Cast<APortfolio_GlobalCharacter>(HitResult.GetActor());

			if (BoneName == "Head") // ��弦( +75% )
			{
				if (HitCharacter)
				{
					float ExtraDamage = Damage * 0.75;
					Damage += ExtraDamage;
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "pelvis" || BoneName == "spine_01" || BoneName == "spine_02") // ���뼦( +20% )
			{
				if (HitCharacter)
				{
					float ExtraDamage = Damage * 0.20;
					Damage += ExtraDamage;
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "upperarm_l" || BoneName == "lowerarm_l" || BoneName == "hand_l") // ����( )
			{
				if (HitCharacter)
				{
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "upperarm_r" || BoneName == "lowerarm_r" || BoneName == "hand_r") // ������( )
			{
				if (HitCharacter)
				{
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "thigh_l" || BoneName == "calf_l" || BoneName == "foot_l") // ���ʴٸ�( )
			{
				if (HitCharacter)
				{
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "thigh_r" || BoneName == "calf_r" || BoneName == "foot_r") // �����ʴٸ�( )
			{
				if (HitCharacter)
				{
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			//else // �׿�
			//{
			//	if (HitCharacter)
			//	{
			//		HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
			//		
			//	}
			//}
		}


	}

}


void AEventActor::OtherDamage(FHitResult HitResult_Other) // ���߿� �ٲ����. �������� ó���ϴ� cpp�� ó���Ѵ�.
{
	float Damage = 100;
	FVector ImpactPoint_Other = HitResult_Other.ImpactPoint;
	HitResult_Other.GetActor();
	UPrimitiveComponent* HitComponent_Other = HitResult_Other.GetComponent();

	FName BoneName_Other = HitResult_Other.BoneName; // �ٵ� �ݸ����� �� �̸��� �����ɴϴ�.

	APortfolio_GlobalCharacter* HitCharacter_Other = Cast<APortfolio_GlobalCharacter>(HitResult_Other.GetActor());

	if (BoneName_Other == "Head") // ��弦( +75% )
	{
		if (HitCharacter_Other)
		{
			float ExtraDamage = Damage * 0.75;
			Damage += ExtraDamage;
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "pelvis" || BoneName_Other == "spine_01" || BoneName_Other == "spine_02") // ���뼦( +20% )
	{
		if (HitCharacter_Other)
		{
			float ExtraDamage = Damage * 0.20;
			Damage += ExtraDamage;
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "upperarm_l" || BoneName_Other == "lowerarm_l" || BoneName_Other == "hand_l") // ����( )
	{
		if (HitCharacter_Other)
		{
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "upperarm_r" || BoneName_Other == "lowerarm_r" || BoneName_Other == "hand_r") // ������( )
	{
		if (HitCharacter_Other)
		{
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "thigh_l" || BoneName_Other == "calf_l" || BoneName_Other == "foot_l") // ���ʴٸ�( )
	{
		if (HitCharacter_Other)
		{
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "thigh_r" || BoneName_Other == "calf_r" || BoneName_Other == "foot_r") // �����ʴٸ�( )
	{
		if (HitCharacter_Other)
		{
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}

}