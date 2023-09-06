// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

//¹«±â Enum
UENUM(BlueprintType)
enum class EEquipedWeapon : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Sword UMETA(DisplayName = "Sword"),
	DoubleAxe UMETA(DisplayName = "DoubleAxe"),

	Max UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"),
	AOE UMETA(DisplayName = "AOE"),
	AOE_Player_Center UMETA(DisplayName = "AOE Player Center"),

	Max UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EEnemyPattern : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Chase UMETA(DisplayName = "Chase"),
	MiddleRange UMETA(DisplayName = "MiddleRange"),
	CloseRange UMETA(DisplayName = "CloseRange"),

	Max UMETA(DisplayName = "Max")
};