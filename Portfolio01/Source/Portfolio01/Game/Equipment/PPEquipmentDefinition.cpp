// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPEquipmentDefinition.h"
#include "PPEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPEquipmentDefinition)

UPPEquipmentDefinition::UPPEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UPPEquipmentInstance::StaticClass();
}

