// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"

class UObject;

PPGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogPP, Log, All);
PPGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogPPExperience, Log, All);
PPGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogPPAbilitySystem, Log, All);
PPGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogPPTeams, Log, All);

PPGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
