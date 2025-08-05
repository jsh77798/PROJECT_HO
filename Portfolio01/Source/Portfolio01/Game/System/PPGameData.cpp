// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPGameData.h"
#include "PPAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPGameData)

UPPGameData::UPPGameData()
{
}

const UPPGameData& UPPGameData::UPPGameData::Get()
{
	return UPPAssetManager::Get().GetGameData();
}
