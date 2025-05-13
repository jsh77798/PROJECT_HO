// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"

class UObject;

PORTFOLIO01_API DECLARE_LOG_CATEGORY_EXTERN(LogPP, Log, All);
PORTFOLIO01_API DECLARE_LOG_CATEGORY_EXTERN(LogPPExperience, Log, All);
PORTFOLIO01_API DECLARE_LOG_CATEGORY_EXTERN(LogPPAbilitySystem, Log, All);
PORTFOLIO01_API DECLARE_LOG_CATEGORY_EXTERN(LogPPTeams, Log, All);

PORTFOLIO01_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
