// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPUICameraManagerComponent.h"

#include "Delegates/Delegate.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "PPPlayerCameraManager.h"
#include "Misc/CoreMisc.h"
#include "Templates/Casts.h"
#include "Templates/UnrealTemplate.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPUICameraManagerComponent)

class AActor;
class FDebugDisplayInfo;

UPPUICameraManagerComponent* UPPUICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (APPPlayerCameraManager* PCCamera = Cast<APPPlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

UPPUICameraManagerComponent::UPPUICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void UPPUICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UPPUICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<APPPlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UPPUICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UPPUICameraManagerComponent::UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime)
{
}

void UPPUICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
}
