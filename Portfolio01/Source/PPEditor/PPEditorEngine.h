// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Editor/UnrealEdEngine.h"
#include "Engine/GameInstance.h"
#include "HAL/Platform.h"
#include "UObject/UObjectGlobals.h"

#include "PPEditorEngine.generated.h"

class IEngineLoop;
class UObject;


UCLASS()
class UPPEditorEngine : public UUnrealEdEngine
{
	GENERATED_BODY()

public:

	UPPEditorEngine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void Init(IEngineLoop* InEngineLoop) override;
	virtual void Start() override;
	virtual void Tick(float DeltaSeconds, bool bIdleMode) override;
	
	virtual FGameInstancePIEResult PreCreatePIEInstances(const bool bAnyBlueprintErrors, const bool bStartInSpectatorMode, const float PIEStartTime, const bool bSupportsOnlinePIE, int32& InNumOnlinePIEInstances) override;

private:
	void FirstTickSetup();
	
	bool bFirstTickSetup = false;
};
