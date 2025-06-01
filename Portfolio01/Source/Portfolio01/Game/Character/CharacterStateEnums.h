#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EnumType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	PLAYER UMETA(DisplayName = "PLAYER"),
	ENEMY UMETA(DisplayName = "ENEMY"),
};

UENUM(BlueprintType)
enum class Player_State : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	IDLE UMETA(DisplayName = "IDLE"),
	MOVE UMETA(DisplayName = "MOVE"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	DEATH UMETA(DisplayName = "DEATH"),
	STUN UMETA(DisplayName = "STUN"),
	LEFT_LEG_STUN UMETA(DisplayName = "LEFT_LEG_STUN"),
	RIGHT_LEG_STUN UMETA(DisplayName = "RIGHT_LEG_STUN"),
	ABDOMEN_STUN UMETA(DisplayName = "ABDOMEN_STUN"),
	LEFT_ARM_STUN UMETA(DisplayName = "LEFT_ARM_STUN"),
	RIGHT_ARM_STUN UMETA(DisplayName = "RIGHT_ARM_STUN"),
};

UENUM(BlueprintType)
enum class Enemy_State : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	IDLE UMETA(DisplayName = "IDLE"),
	MOVE UMETA(DisplayName = "MOVE"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	DEATH UMETA(DisplayName = "DEATH"),
	STUN UMETA(DisplayName = "STUN"),
	LEFT_LEG_STUN UMETA(DisplayName = "LEFT_LEG_STUN"),
	RIGHT_LEG_STUN UMETA(DisplayName = "RIGHT_LEG_STUN"),
	ABDOMEN_STUN UMETA(DisplayName = "ABDOMEN_STUN"),
	LEFT_ARM_STUN UMETA(DisplayName = "LEFT_ARM_STUN"),
	RIGHT_ARM_STUN UMETA(DisplayName = "RIGHT_ARM_STUN"),
};
