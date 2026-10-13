// Fill out your copyright notice in the Description page of Project Settings.


#include "Portfolio_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Portfolio_HUD.h"
#include "UI/Portfolio_Widget_Main.h"
#include "UI/Portfolio_Widget_Inventory.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APortfolio_Character::APortfolio_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 25.f;
	BaseLookUpRate = 25.f;

	//캐릭터 기본 이동속도 설정
	MoveCom = Cast<UCharacterMovementComponent>(GetMovementComponent());
	MoveCom->MaxWalkSpeed = GetSpeed();  
	
	//Create our components 스프링암 설정
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 140.0f;
	OurCameraSpringArm->SocketOffset.Y = 55.0f;
	OurCameraSpringArm->SocketOffset.Z = 65.0f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 10.0f;

	//카메라 설정(앞에 스프링암에 붙여줌)
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(OurCameraSpringArm);

	
	//Take control of the default Player
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	//캐릭터 이동 회전 (#include "GameFramework/CharacterMovementComponent.h" 헤더 필요)
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

UPortfolio_Widget_Inventory* APortfolio_Character::GetInventoryWidget() const
{
	APlayerController* PC =
		Cast<APlayerController>(GetController());

	if (!IsValid(PC))
	{
		return nullptr;
	}

	APortfolio_HUD* HUD = PC->GetHUD<APortfolio_HUD>();

	if (!IsValid(HUD))
	{
		return nullptr;
	}

	UPortfolio_Widget_Main* MainWidget = HUD->GetMainWidget();

	if (!IsValid(MainWidget))
	{
		return nullptr;
	}

	return Cast<UPortfolio_Widget_Inventory>(
		MainWidget->GetWidgetFromName(TEXT("WBP_UI_Inventory"))
	);
}

void APortfolio_Character::SyncInventoryAmmo()
{
	//if (!CurPlayerData)
	//{
	//	return;
	//}
	//
	//UPortfolio_Widget_Inventory* Inventory = GetInventoryWidget();
	//
	//const int32 ReserveAmmo =
	//	IsValid(Inventory) ? Inventory->GetAmmoCount() : 0;
	//
	//CurPlayerData->Bullet = ReserveAmmo;
	//Bullet = ReserveAmmo;
	//LoadBullet = CurPlayerData->LoadBullet;

	UPortfolio_Widget_Inventory* Inventory = GetInventoryWidget();

	Bullet = IsValid(Inventory)
		? Inventory->GetAmmoCountForRow(GetCurrentAmmoRow())
		: 0;

	LoadBullet = GetLoadedAmmoRef();

	// 기존 UI나 코드와의 호환을 위한 현재 무기 값
	if (CurPlayerData)
	{
		CurPlayerData->Bullet = Bullet;
		CurPlayerData->LoadBullet = LoadBullet;
	}
}

void APortfolio_Character::EquipShotgun()
{
	EquipWeapon(EWeaponType::Shotgun);
}

void APortfolio_Character::EquipPistol()
{
	EquipWeapon(EWeaponType::Pistol);
}

void APortfolio_Character::EquipWeapon(EWeaponType NewWeapon)
{
	const EAniState State = GetAniState<EAniState>();

	// 동작 중 교체 금지
	if (State == EAniState::W_Attack ||
		State == EAniState::Load ||
		State == EAniState::Hit ||
		State == EAniState::CrouchOn ||
		State == EAniState::CrouchOff)
	{
		return;
	}

	auto* AnimInstance = GetGlobalAnimInstance();

	if (!IsValid(AnimInstance))
	{
		return;
	}

	// 피스톨 필수 설정 확인
	if (NewWeapon == EWeaponType::Pistol)
	{
		const EAniState RequiredStates[] =
		{
			EAniState::Idle,
			EAniState::W_Aiming,
			EAniState::W_Attack,
			EAniState::Load
		};

		for (EAniState RequiredState : RequiredStates)
		{
			UAnimMontage* const* Montage =
				PistolMapAnimation.Find(RequiredState);

			if (!Montage || !IsValid(*Montage))
			{
				UE_LOG(
					LogTemp, Warning,
					TEXT("Pistol montage missing. State=%d"),
					static_cast<int32>(RequiredState)
				);

				return;
			}
		}

		if (!PistolProjectileClass)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("PistolProjectileClass is not assigned."));
			return;
		}
	}

	// 기존 동작을 종료한 뒤 새 몽타주 목록 적용
	AnimInstance->Montage_Stop(0.0f);

	CurrentWeapon = NewWeapon;

	TMap<EAniState, UAnimMontage*> ActiveAnimations = MapAnimation;

	if (CurrentWeapon == EWeaponType::Pistol)
	{
		for (const auto& Pair : PistolMapAnimation)
		{
			if (IsValid(Pair.Value))
			{
				ActiveAnimations.Add(Pair.Key, Pair.Value);
			}
		}
	}

	// 캐릭터가 조회하는 목록 갱신
	SetAllAnimation(ActiveAnimations);

	// 실제 재생을 담당하는 AnimInstance 목록도 갱신
	AnimInstance->AllAnimations.Empty();

	for (const auto& Pair : ActiveAnimations)
	{
		AnimInstance->AllAnimations.Add(
			static_cast<int32>(Pair.Key),
			Pair.Value
		);
	}

	AnimInstance->CurMontage = nullptr;

	// 태그가 지정된 무기 메시 표시 전환
	auto SetWeaponVisible = [&](FName ComponentTag, bool bVisible)
		{
			const TArray<UActorComponent*> Components =
				GetComponentsByTag(
					USceneComponent::StaticClass(),
					ComponentTag
				);

			for (UActorComponent* Component : Components)
			{
				if (USceneComponent* SceneComponent =
					Cast<USceneComponent>(Component))
				{
					SceneComponent->SetVisibility(bVisible, true);
					SceneComponent->SetHiddenInGame(!bVisible, true);
				}
			}
		};

	SetWeaponVisible(
		TEXT("WeaponMesh"),
		CurrentWeapon == EWeaponType::Shotgun
	);

	SetWeaponVisible(
		TEXT("PistolMesh"),
		CurrentWeapon == EWeaponType::Pistol
	);

	bShotPending = false;
	bPistolReloadApplied = false;

	RunCheck = 0;
	RunZooming = false;
	CrouchCheck = 0;
	CrouchZooming = false;

	SetAniState(
		AimingCheck ? EAniState::W_Aiming : EAniState::Idle
	);

	SyncInventoryAmmo();
}

int32 APortfolio_Character::GetMaxAmmo() const
{
	return FMath::Max(
		1,
		CurrentWeapon == EWeaponType::Shotgun
		? ShotgunMaxAmmo
		: PistolMaxAmmo
	);
}

int32& APortfolio_Character::GetLoadedAmmoRef()
{
	return CurrentWeapon == EWeaponType::Shotgun
		? ShotgunLoadedAmmo
		: PistolLoadedAmmo;
}

FDataTableRowHandle APortfolio_Character::GetCurrentAmmoRow() const
{
	UPortfolio_Widget_Inventory* Inventory = GetInventoryWidget();

	if (!IsValid(Inventory))
	{
		return FDataTableRowHandle();
	}

	return CurrentWeapon == EWeaponType::Shotgun
		? Inventory->AmmoItemRow
		: Inventory->PistolAmmoItemRow;
}

// Called when the game starts or when spawned
void APortfolio_Character::BeginPlay()
{
	SetAllAnimation(MapAnimation);

	Super::BeginPlay();

	
	// PlayerData에서 공격력을 가져와 저장한다
	UPortfolio_GameInstance* Inst = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();
	if (nullptr != Inst)
	{
		CurPlayerData = Inst->GetPlayerData(PlayerDataName);
	}

	if (CurPlayerData)
	{
		PlayerAtt = CurPlayerData->ATT;
		Damage(PlayerAtt);

		ShotgunLoadedAmmo = FMath::Clamp(
			CurPlayerData->LoadBullet,
			0,
			FMath::Max(1, ShotgunMaxAmmo)
		);
	}

	PistolLoadedAmmo = 0;

	GetGlobalAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &APortfolio_Character::MontageEnd);
	GetGlobalAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &APortfolio_Character::AnimNotifyBegin);
	SetAniState(EAniState::Idle);
	EquipWeapon(EWeaponType::Shotgun);
}

// Called every frame
void APortfolio_Character::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
	CameraLoc = camera->GetComponentLocation();
	CameraForward = camera->GetForwardVector();

	AniStateValue = GetAniState<EAniState>();

	SyncInventoryAmmo();
	
	if (GetItem == true)
	{
		GetitemCheck = true;
	}

	if (GetitemCheck == true && Item_Get_InputCheck == true)
	{
		//CurPlayerData->Bullet += 1;
		Item_Get_InputCheck = false;
	}
	else 
	{
	    Item_Get_InputCheck = false;
	}

	{
        AttackHitCheck = GetHit();
		if (AttackHitCheck == true) 
		{
			SetAniState(EAniState::Hit);
			SetAttackAnimcheck(false);
		}
	}

	//Run 상태확인 및 설정
	{
	    if (AniStateValue == EAniState::Idle)
	    {
	    	RunCheck = 0;
			RunZooming = false;
	    }
	}

	//use controller rotation yaw 설정
	{
		if (AniStateValue == EAniState::Idle || AniStateValue == EAniState::ForwardMove || RunCheck == 1)
		{
			bUseControllerRotationYaw = false;

			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
		}
		else 
		{
		    bUseControllerRotationYaw = true;
		}
	}

	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		if (bZoomingIn || RunZooming || CrouchZooming)
		{
			ZoomFactor += DeltaTime / S;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / S;        //Zoom out over a quarter of a second
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		//FMath::Lerp<float>(Af, Bf, C) -> C의 속력으로 A-B를 한다.
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(140.0f, A, ZoomFactor);
		OurCameraSpringArm->SocketOffset.Y = FMath::Lerp<float>(55.0f, B, ZoomFactor);
		OurCameraSpringArm->SocketOffset.Z = FMath::Lerp<float>(65.0f, C, ZoomFactor);
	}

	//공격에서 AttackCheck == 1이 되면 여기서 공격실행
	//{
	//	if (AttackCheck == 1) {
	//
	//        if (AimingActionCheck == 1 && AniStateValue == EAniState::W_Aiming)
	//        {
	//			SetAniState(EAniState::W_Attack);
	//
	//			//AimingAttack();
	//			CurPlayerData->LoadBullet -= 1;
	//		    AttackCheck= 0;
	//        }
 	//
	//        if (AimingActionCheck == 1
	//	        || AniStateValue == EAniState::Aiming_ForwardMove
	//	        || AniStateValue == EAniState::Aiming_BackwardMove
	//	        || AniStateValue == EAniState::Aiming_RightMove
	//	        || AniStateValue == EAniState::Aiming_LeftMove)
	//        {
	//			SetAniState(EAniState::W_Attack);
	//			AttackCheck= 0;
	//        }
	//	}
	//}
}


void APortfolio_Character::ZoomCheck(float *_A, float *_B, float *_C, float* _S)
{
	A = *_A;
	B = *_B;
	C = *_C;
	S = *_S;
}

void APortfolio_Character::MontageEnd(UAnimMontage* Anim, bool _Inter)
{
	if (!Anim)
	{
		return;
	}

	const EAniState State = GetAniState<EAniState>();

	if (Anim == GetAnimMontage(EAniState::Load))
	{
		bPistolReloadApplied = false;

		if (State == EAniState::Load)
		{
			SetAniState(
				AimingCheck ? EAniState::W_Aiming : EAniState::Idle
			);
		}

		return;
	}

	if (Anim == GetAnimMontage(EAniState::W_Attack))
	{
		bShotPending = false;

		if (State == EAniState::W_Attack)
		{
			SetAniState(
				AimingCheck ? EAniState::W_Aiming : EAniState::Idle
			);
		}

		return;
	}

	if (Anim == GetAnimMontage(EAniState::CrouchOn) &&
		State == EAniState::CrouchOn)
	{
		SetAniState(EAniState::Crouch_Idle);
		return;
	}

	if (Anim == GetAnimMontage(EAniState::CrouchOff) &&
		State == EAniState::CrouchOff)
	{
		SetAniState(EAniState::Idle);
		return;
	}

	if (Anim == GetAnimMontage(EAniState::Hit) &&
		State == EAniState::Hit)
	{
		SetAniState(
			AimingCheck ? EAniState::W_Aiming : EAniState::Idle
		);
	}
}

// Called to bind functionality to input
void APortfolio_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	static bool bBindingsAdded = false;

	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		// 축매핑
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::S, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::D, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerTurn", EKeys::MouseX, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLookUp", EKeys::MouseY, -1.f));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("item_Get"), EKeys::X));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerLoad"), EKeys::R));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerAiming"), EKeys::RightMouseButton));
    	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerAttack"), EKeys::LeftMouseButton));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerRun"), EKeys::LeftShift));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerCrouch"), EKeys::Q));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("StatusWindow"), EKeys::Zero));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("InventoryWindow"), EKeys::Nine));

		UPlayerInput::AddEngineDefinedActionMapping(
			FInputActionKeyMapping(TEXT("EquipShotgun"), EKeys::One)
		);

		UPlayerInput::AddEngineDefinedActionMapping(
			FInputActionKeyMapping(TEXT("EquipPistol"), EKeys::Two)
		);
	}

	// 키와 함수를 연결한다
	// 해당키가 눌리면 이 함수를 실행시킨다
	// BindAction_축일때는 한번 실행시킨다
	// BindAxis_축일때는 계속 실행시킨다
	PlayerInputComponent->BindAxis("PlayerMoveForward", this, &APortfolio_Character::MoveForward);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &APortfolio_Character::MoveRight);
	PlayerInputComponent->BindAxis("PlayerTurn", this, &APortfolio_Character::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PlayerTurnRate", this, &APortfolio_Character::TurnAtRate);
	PlayerInputComponent->BindAxis("PlayerLookUp", this, &APortfolio_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("PlayerLookUpRate", this, &APortfolio_Character::LookUpAtRate);

	PlayerInputComponent->BindAction("item_Get", EInputEvent::IE_Pressed, this, &APortfolio_Character::item_Get_check);

	PlayerInputComponent->BindAction("PlayerLoad", EInputEvent::IE_Pressed, this, &APortfolio_Character::LoadAction);
	PlayerInputComponent->BindAction("PlayerAiming", EInputEvent::IE_Pressed , this, &APortfolio_Character::IN_AimingAction);
	PlayerInputComponent->BindAction("PlayerAiming", EInputEvent::IE_Released, this, &APortfolio_Character::OUT_AimingAction);
	PlayerInputComponent->BindAction("PlayerAttack", EInputEvent::IE_Pressed, this, &APortfolio_Character::AttackAction);

	PlayerInputComponent->BindAction("PlayerRun", EInputEvent::IE_Pressed, this, &APortfolio_Character::Run);
	PlayerInputComponent->BindAction("PlayerCrouch", EInputEvent::IE_Pressed, this, &APortfolio_Character::Crouch);
	
	PlayerInputComponent->BindAction(
		TEXT("EquipShotgun"),
		IE_Pressed,
		this,
		&APortfolio_Character::EquipShotgun
	);

	PlayerInputComponent->BindAction(
		TEXT("EquipPistol"),
		IE_Pressed,
		this,
		&APortfolio_Character::EquipPistol
	);
}

//좌우이동
void APortfolio_Character::MoveRight(float Val)
{
	AniStateValue = GetAniState<EAniState>();

	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();
			// transform to world space and add it
			// 현재 내 회전을 가져와서 y축에 해당하는 축벡터를 얻어오는 겁니다.
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);

			// 이동은 적용하고 공격·장전 애니메이션은 유지
			const EAniState State = GetAniState<EAniState>();

			if (State == EAniState::W_Attack ||
				State == EAniState::Load)
			{
				return;
			}

			//달리기 이동
			if (RunCheck == 1 && AimingActionCheck == 0 && AniStateValue != EAniState::Idle)
			{
				MoveCom->MaxWalkSpeed = GetSpeed() * 1.9f; // 150%
				RunZooming = true;
				SetAniState(Val > 0.f ? EAniState::RightRun : EAniState::LeftRun);
				return;
			}
			
			//웅크리기 이동
			if (CrouchCheck == 1)
			{
				MoveCom->MaxWalkSpeed = GetSpeed() * 0.8f; // 80%
				SetAniState(Val > 0.f ? EAniState::Crouch_RightMove : EAniState::Crouch_LeftMove);
				return;
			}

			//조준이동 및 기본이동
		    {
			    if (AimingActionCheck == 0) 
			    {
			  	    MoveCom->MaxWalkSpeed = GetSpeed(); 
					SetAniState(Val > 0.f ? EAniState::RightMove : EAniState::LeftMove);
				    return;
			    }
			    else 
			    {
			    	MoveCom->MaxWalkSpeed = GetSpeed() * 0.7f; // 70%
					SetAniState(Val > 0.f ? EAniState::Aiming_RightMove : EAniState::Aiming_LeftMove);
				    return;
		  	    }  
		    }


		}
	}
	else
	{
		if (AniStateValue == EAniState::RightMove || AniStateValue == EAniState::LeftMove )
		{
			SetAniState(EAniState::Idle);
		}
		if (AniStateValue == EAniState::Aiming_RightMove || AniStateValue == EAniState::Aiming_LeftMove )
		{
			SetAniState(EAniState::W_Aiming);
		}

		if (AniStateValue == EAniState::RightRun || AniStateValue == EAniState::LeftRun)
		{
			SetAniState(EAniState::Idle);
			RunZooming = false;
			RunCheck = 0;
		}

		if (AniStateValue == EAniState::Crouch_RightMove || AniStateValue == EAniState::Crouch_LeftMove)
		{
			SetAniState(EAniState::Crouch_Idle);
		}
	}
}

//앞뒤이동
void APortfolio_Character::MoveForward(float Val)
{
	AniStateValue = GetAniState<EAniState>();

	if (Val != 0.f)
	{

		if (Controller)
		{
			
			FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
			AddMovementInput(Direction, Val);

			// 이동은 적용하고 공격·장전 애니메이션은 유지
			const EAniState State = GetAniState<EAniState>();

			if (State == EAniState::W_Attack ||
				State == EAniState::Load)
			{
				return;
			}

			//달리기 이동
			if (RunCheck == 1 && AimingActionCheck == 0 && AniStateValue != EAniState::Idle)
			{
				MoveCom->MaxWalkSpeed = GetSpeed() * 1.9f; // 150%
				RunZooming = true;
				SetAniState(Val > 0.f ? EAniState::ForwardRun : EAniState::BackwardRun);
				return;
			}

			//웅크리기 이동
			if (CrouchCheck == 1)
			{
				MoveCom->MaxWalkSpeed = GetSpeed() * 0.8f; // 80%
				SetAniState(Val > 0.f ? EAniState::Crouch_ForwardMove : EAniState::Crouch_BackwardMove);
				return;
			}

			//조준이동 및 기본이동
			{
				if (AimingActionCheck == 0)
				{
					MoveCom->MaxWalkSpeed = GetSpeed(); 
					SetAniState(Val > 0.f ? EAniState::ForwardMove : EAniState::BackwardMove);
					return;
				}
				else
				{
					MoveCom->MaxWalkSpeed = GetSpeed() * 0.7f; // 70%
					float a = 60.0f; //타겟암 길이
					float b = 80.0f; //타겟암 Y축
					float c = 70.0f; //타겟암 Z축
					float s = 0.15f; //속력
					ZoomCheck(&a, &b, &c, &s); //타겟암 설정
					SetAniState(Val > 0.f ? EAniState::Aiming_ForwardMove : EAniState::Aiming_BackwardMove);
					return;
				}
			}

		}
	}
	else
	{
		if (AniStateValue == EAniState::ForwardMove || AniStateValue == EAniState::BackwardMove )
		{
			SetAniState(EAniState::Idle);
		}
		if (AniStateValue == EAniState::Aiming_ForwardMove || AniStateValue == EAniState::Aiming_BackwardMove)
		{
			SetAniState(EAniState::W_Aiming);
		}

		if (AniStateValue == EAniState::ForwardRun || AniStateValue == EAniState::BackwardRun)
		{
			SetAniState(EAniState::Idle);
			RunZooming = false;
			RunCheck = 0;
		}

		if (AniStateValue == EAniState::Crouch_ForwardMove || AniStateValue == EAniState::Crouch_BackwardMove)
		{
			SetAniState(EAniState::Crouch_Idle);
		}
	}
}

void APortfolio_Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void APortfolio_Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

//장전
void APortfolio_Character::LoadAction()
{
	if (!CurPlayerData)
	{
		return;
	}

	const EAniState State = GetAniState<EAniState>();

	if (State == EAniState::Load ||
		State == EAniState::W_Attack ||
		State == EAniState::Hit)
	{
		return;
	}

	// 검사 전에 최신 수량으로 갱신
	SyncInventoryAmmo();

	if (GetLoadedAmmoRef() >= GetMaxAmmo() || Bullet <= 0)
	{
		return;
	}

	bPistolReloadApplied = false;
	SetAniState(EAniState::Load);
}

//조준
void APortfolio_Character::IN_AimingAction()
{
	const EAniState State = GetAniState<EAniState>();

	if (State != EAniState::Load &&
		State != EAniState::W_Attack &&
		State != EAniState::Hit)
	{
		SetAniState(EAniState::W_Aiming);
	}

	SetAimingCheck(true);

	float a = 80.0f; //타겟암 길이
	float b = 80.0f; //타겟암 Y축
	float c = 70.0f; //타겟암 Z축
	float s = 0.15f; //속력
	ZoomCheck(&a, &b, &c, &s); //타겟암 설정

	RunCheck = 0;
	CrouchCheck = 0;
	CrouchZooming = false;

	AimingActionCheck = 1;
	//ZoomingIn = 1;
	bZoomingIn = true;
}

void APortfolio_Character::OUT_AimingAction()
{
	AimingActionCheck = 0;
	SetAimingCheck(false);
	bZoomingIn = false;

	const EAniState State = GetAniState<EAniState>();

	if (State != EAniState::Load &&
		State != EAniState::W_Attack &&
		State != EAniState::Hit)
	{
		SetAniState(EAniState::Idle);
	}
}

//공격
void APortfolio_Character::AttackAction()
{
	if (!CurPlayerData || AimingActionCheck != 1)
	{
		return;
	}

	const EAniState State = GetAniState<EAniState>();

	const bool bCanAttack =
		State == EAniState::W_Aiming ||
		State == EAniState::Aiming_ForwardMove ||
		State == EAniState::Aiming_BackwardMove ||
		State == EAniState::Aiming_RightMove ||
		State == EAniState::Aiming_LeftMove;

	if (!bCanAttack)
	{
		return;
	}

	if (GetLoadedAmmoRef() <= 0)
	{
		LoadAction();
		return;
	}

	--GetLoadedAmmoRef();
	bShotPending = true;

	SetAniState(EAniState::W_Attack);
	SyncInventoryAmmo();
}

//이펙트(공격)
void APortfolio_Character::AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (!CurPlayerData)
	{
		return;
	}

	// 샷건: 한 발씩 장전
	if (NotifyName == FName(TEXT("ReloadShell")))
	{
		if (CurrentWeapon != EWeaponType::Shotgun ||
			GetAniState<EAniState>() != EAniState::Load)
		{
			return;
		}

		UPortfolio_Widget_Inventory* Inventory = GetInventoryWidget();

		if (GetLoadedAmmoRef() < GetMaxAmmo() &&
			IsValid(Inventory) &&
			Inventory->TryConsumeAmmoForRow(
				GetCurrentAmmoRow(), 1))
		{
			++GetLoadedAmmoRef();
		}

		SyncInventoryAmmo();

		if (GetLoadedAmmoRef() >= GetMaxAmmo() || Bullet <= 0)
		{
			UAnimMontage* ReloadMontage =
				GetAnimMontage(EAniState::Load);

			SetAniState(
				AimingCheck ? EAniState::W_Aiming : EAniState::Idle
			);

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (IsValid(AnimInstance) && IsValid(ReloadMontage))
			{
				AnimInstance->Montage_Stop(0.15f, ReloadMontage);
			}
		}

		return;
	}

	// 피스톨: 탄창 삽입 시 필요한 수량을 한 번에 장전
	if (NotifyName == FName(TEXT("ReloadMagazine")))
	{
		if (CurrentWeapon != EWeaponType::Pistol ||
			GetAniState<EAniState>() != EAniState::Load ||
			bPistolReloadApplied)
		{
			return;
		}

		bPistolReloadApplied = true;

		UPortfolio_Widget_Inventory* Inventory = GetInventoryWidget();

		if (!IsValid(Inventory))
		{
			return;
		}

		const FDataTableRowHandle AmmoRow = GetCurrentAmmoRow();

		const int32 Needed =
			FMath::Max(0, GetMaxAmmo() - GetLoadedAmmoRef());

		const int32 Amount = FMath::Min(
			Needed,
			Inventory->GetAmmoCountForRow(AmmoRow)
		);

		if (Amount > 0 &&
			Inventory->TryConsumeAmmoForRow(AmmoRow, Amount))
		{
			GetLoadedAmmoRef() += Amount;
		}

		SyncInventoryAmmo();

		// 탄창 삽입 후 남은 장전 동작은 끝까지 재생
		return;
	}

	// 발사 이벤트만 아래로 통과
	if (NotifyName != FName(TEXT("Fire")) ||
		GetAniState<EAniState>() != EAniState::W_Attack ||
		!bShotPending)
	{
		return;
	}

	bShotPending = false;

	const FName MuzzleTag =
		CurrentWeapon == EWeaponType::Shotgun
		? FName(TEXT("WeaponEffect"))
		: FName(TEXT("PistolEffect"));

	const TArray<UActorComponent*> MuzzleComponents =
		GetComponentsByTag(USceneComponent::StaticClass(), MuzzleTag);

	if (MuzzleComponents.IsEmpty())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Weapon muzzle component is missing."));
		return;
	}

	USceneComponent* Muzzle =
		Cast<USceneComponent>(MuzzleComponents[0]);

	if (!IsValid(Muzzle))
	{
		return;
	}

	UClass* ProjectileClass = nullptr;

	if (CurrentWeapon == EWeaponType::Pistol)
	{
		ProjectileClass = PistolProjectileClass.Get();
	}
	else
	{
		auto* Inst =
			GetWorld()->GetGameInstance<UPortfolio_GameInstance>();

		if (IsValid(Inst))
		{
			ProjectileClass =
				Inst->GetSubClass(TEXT("PlayerRangeAttack")).Get();
		}
	}

	if (!ProjectileClass ||
		!ProjectileClass->IsChildOf(APortfolio_Tile::StaticClass()))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;

	APortfolio_Tile* Projectile =
		GetWorld()->SpawnActor<APortfolio_Tile>(
			ProjectileClass,
			Muzzle->GetComponentLocation(),
			camera->GetComponentRotation(),
			SpawnParameters
		);

	if (!IsValid(Projectile))
	{
		return;
	}

	const FVector AimLocation = camera->GetComponentLocation();
	const FVector AimDirection = camera->GetForwardVector();

	if (CurrentWeapon == EWeaponType::Pistol)
	{
		// 피스톨 피해는 트레이스로만 처리
		// 발사체의 오버랩으로 추가 피해가 발생하는 것을 방지
		Projectile->SetActorEnableCollision(false);
		Projectile->Tags.Remove(FName(TEXT("Damage")));

		if (USphereComponent* Sphere =
			Projectile->GetSphereComponent())
		{
			Sphere->SetCollisionEnabled(
				ECollisionEnabled::NoCollision
			);

			Sphere->ComponentTags.Remove(
				FName(TEXT("Damage"))
			);
		}

		Projectile->PistolAttack(AimLocation, AimDirection);
	}
	else
	{
		// 샷건의 기존 공격 처리
		Projectile->Tags.AddUnique(TEXT("Damage"));

		if (Projectile->GetSphereComponent())
		{
			Projectile->GetSphereComponent()->SetCollisionProfileName(
				TEXT("PlayerAttack"), true
			);
		}

		Projectile->AimingAttack(AimLocation, AimDirection);
		Projectile->PerformSweep(AimLocation, AimDirection);
	}
}

//달리기
void APortfolio_Character::Run() 
{
	const EAniState State = GetAniState<EAniState>();

	if (State == EAniState::Load ||
		State == EAniState::W_Attack ||
		State == EAniState::Hit)
	{
		return;
	}

	float a = 145.0f; //타겟암 길이
	float b = 45.0f; //타겟암 Y축
	float c = 60.0f; //타켓암 Z축
	float s = 0.35f; //속력

	if (RunCheck == 0)
	{
		CrouchCheck = 0;
		CrouchZooming = false;

		ZoomCheck(&a, &b, &c, &s); //타겟암 설정
		RunCheck = 1;
		return;
	}
	else
	{
		RunZooming = false;
		RunCheck = 0;
		return;
	}
}

//웅크리기
void APortfolio_Character::Crouch() 
{
	const EAniState State = GetAniState<EAniState>();

	if (State == EAniState::Load ||
		State == EAniState::W_Attack ||
		State == EAniState::Hit)
	{
		return;
	}

	float a = 150.0f; //타겟암 길이
	float b = 55.0f; //타겟암 Y축
	float c = 50.0f; //타켓암 Z축
	float s = 0.35f; //속력

	if (CrouchCheck == 0 && AimingActionCheck == 0)
	{
		RunCheck = 0;
		AimingActionCheck = 0;
		bZoomingIn = false;

		ZoomCheck(&a, &b, &c, &s); //타겟암 설정
		CrouchZooming = true;
	    CrouchCheck = 1;
		SetAniState(EAniState::CrouchOn);
		return;
	}
	else
	{
		SetAniState(EAniState::CrouchOff);
		CrouchZooming = false;
		CrouchCheck = 0;
		return;
	}
}

void APortfolio_Character::item_Get_check()
{
	Item_Get_InputCheck = true;
}

void APortfolio_Character::AnimationTick()
{

}

/*
void APortfolio_Character::BulletPlus()
{
	CurPlayerData->Bullet += 1;
}
*/

/*
void APortfolio_Character::AimingAttack() 
{

	CameraLoc = camera->GetComponentLocation();
	CameraForward = camera->GetForwardVector();
	FVector StartLoc = CameraLoc; // 레이저 시작 지점.
	FVector EndLoc = CameraLoc + (CameraForward * 5000.0f); // 레이저 끝나는 지점.


	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
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

	TArray<AActor*> IgnoreActors; // 무시할 액터들.

	FHitResult HitResult; // 히트 결과 값 받을 변수.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		// 여기 밑에 3개는 기본 값으로 제공됨. 바꾸려면 적으면 됨.
		//, FLinearColor::Red
		//, FLinearColor::Green
		//, 5.0f
	);

	if (Result == true)
	{
		FVector ImpactPoint = HitResult.ImpactPoint;
		// HitResult에서 필요한 값들을 사용하면 됨.
	}
}
*/