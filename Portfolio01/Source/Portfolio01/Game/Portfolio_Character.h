// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/Portfolio_GlobalCharacter.h"
#include <Global/Portfolio_GameInstance.h>
#include <Global/Data/PlayerData.h>
#include <Global/Portfolio_Tile.h>
#include <UI/Portfolio_Widget_Main.h>
#include "Portfolio_Enums.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/DataTable.h"
#include "Portfolio_Character.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Shotgun,
	Pistol
};

UCLASS()
class PORTFOLIO01_API APortfolio_Character : public APortfolio_GlobalCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APortfolio_Character();

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Bullet = 0;

	UPROPERTY(Category = "Bullet", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int LoadBullet = 0;

	UFUNCTION(BlueprintPure, Category = "Character")
	float GetCharacterBullet() const
	{
		return Bullet; // Health는 ACharacter 클래스의 멤버 변수로 가정
	}


	// PlayerData에서 데미지 값 가져오기
	UPROPERTY(Category = "ATT", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName PlayerDataName = "NONE";

	struct FPlayerData* CurPlayerData = nullptr;
	int PlayerAtt = 0;

	// 블루프린트에서 호출하게 하려면 기본적으로 public이어야 합니다.
	UFUNCTION(BlueprintCallable)
	void AnimationTick();

	UPROPERTY(Category = "input", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool GetItem = false;

	bool GetitemCheck = false;
	bool Item_Get_InputCheck = false;

	//UFUNCTION(BlueprintCallable)
	//void BulletPlus();

	class UPortfolio_Widget_Inventory* GetInventoryWidget() const;

	void SyncInventoryAmmo();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType CurrentWeapon = EWeaponType::Shotgun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 ShotgunMaxAmmo = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 PistolMaxAmmo = 12;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 ShotgunLoadedAmmo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 PistolLoadedAmmo = 0;

	// 기존 MapAnimation은 샷건용으로 유지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animation")
	TMap<EAniState, UAnimMontage*> PistolMapAnimation;

	// 피스톨에 사용할 발사체 BP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<APortfolio_Tile> PistolProjectileClass;

	void EquipShotgun();
	void EquipPistol();
	void EquipWeapon(EWeaponType NewWeapon);

	int32 GetMaxAmmo() const;
	int32& GetLoadedAmmoRef();

	FDataTableRowHandle GetCurrentAmmoRow() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector CameraLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector CameraForward;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	bool AimingCheck;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetAimingCheck(const bool& Check)
	{
		AimingCheck = Check;
	}

	void MoveRight(float Val);
	void MoveForward(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void LoadAction();
	void IN_AimingAction();
	void OUT_AimingAction();
	void AttackAction();

	void item_Get_check();
	
	void Run();
	void Crouch();

	void ZoomCheck(float *_A, float *_B, float *_C, float* _S);
	float A; //타겟암 길이
	float B; //타겟암 Y축
	float C; //타겟암 Z축
	float S; //속력

	int RunCheck = 0;
	bool RunZooming;

	int CrouchCheck = 0;
	bool CrouchZooming;
	

	int AimingActionCheck = 0;
	int AttackCheck = 0;

	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;

	UCharacterMovementComponent* MoveCom;

	//스프링암
	UPROPERTY(EditAnywhere)
		USpringArmComponent* OurCameraSpringArm;
	//타겟암 길이: 140.0
	//소켓 오프셋: 0.0 / 55.0 / 65.0
	//타깃 오프셋: 0.0 / 0.0 / 0.0
	
	//UCameraComponent* OurCamera;
    //카메라
	UPROPERTY(EditAnyWhere, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* camera;

	EAniState AniStateValue;

	UPROPERTY(Category = "AttackHitCheck", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	    bool AttackHitCheck;

	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<EAniState, class UAnimMontage*> MapAnimation;

	UFUNCTION()
		void MontageEnd(UAnimMontage* Anim, bool _Inter);

	UFUNCTION()
		void AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

    

	float Speed = 1500.0f;

	//공격시 라인 트레이싱 함수 실행
	/*
	UFUNCTION()
	    void AimingAttack();
	*/

private:
	//virtual void Tick(float DeltaTime) override;
	void Tick(float _Delta) override;

	// 발사 노티파이가 한 공격에서 중복 실행되는 것을 방지
	bool bShotPending = false;

	// 피스톨 장전 노티파이 중복 처리 방지
	bool bPistolReloadApplied = false;
};

