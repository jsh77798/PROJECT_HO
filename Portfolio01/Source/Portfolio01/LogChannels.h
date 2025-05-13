// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"

class UObject;

PORTFOLIO01_API DECLARE_LOG_CATEGORY_EXTERN(LogPF, Log, All);
PORTFOLIO01_API DECLARE_LOG_CATEGORY_EXTERN(LogPFExperience, Log, All);
PORTFOLIO01_API DECLARE_LOG_CATEGORY_EXTERN(LogPFAbilitySystem, Log, All);
PORTFOLIO01_API DECLARE_LOG_CATEGORY_EXTERN(LogPFTeams, Log, All);

PORTFOLIO01_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
