// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_PPContextEffectsLibrary.h"

#include "Feedback/ContextEffects/PPContextEffectsLibrary.h"

class UClass;

#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_PPContextEffectsLibrary::GetSupportedClass() const
{
	return UPPContextEffectsLibrary::StaticClass();
}

#undef LOCTEXT_NAMESPACE
