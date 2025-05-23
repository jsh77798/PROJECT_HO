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
#include "Portfolio_Character.generated.h"

class UHealthComponent;

UCLASS()
class PORTFOLIO01_API APortfolio_Character : public APortfolio_GlobalCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APortfolio_Character();

	// PlayerData에서 데미지 값 가져오기
	UPROPERTY(Category = "ATT", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName PlayerDataName = "NONE";

	struct FPlayerData* CurPlayerData;
	int PlayerAtt = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
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
	void IN_AimingAction();
	void OUT_AimingAction();
	void AttackAction();

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

	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<EAniState, class UAnimMontage*> MapAnimation;

	UFUNCTION()
		void MontageEnd(UAnimMontage* Anim, bool _Inter);

	UFUNCTION()
		void AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	float Speed = 1500.0f;

private:
	TMap<FName, TSubclassOf<UObject>> Notify;

	//virtual void Tick(float DeltaTime) override;
	void Tick(float _Delta) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;
	
};

