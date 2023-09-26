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
enum class ECombetEffectType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Block UMETA(DisplayName = "Block"),
	Parrying UMETA(DisplayName = "Parrying"),
	Charge UMETA(DisplayName = "EF Charge"),

	Max UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EDamageEffectType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	NormalAttack UMETA(DisplayName = "Normal Attack"),
	LightAttack UMETA(DisplayName = "Light Attack"),
	HeavyAttack UMETA(DisplayName = "Heavy Attack"),
	SpecialAttack UMETA(DisplayName = "Special Attack"),
	PostureUp UMETA(DisplayName = "Posture Up"),
	PostureUp_OnParry UMETA(DisplayName = "Posture Up_OnParry"),

	Max UMETA(DisplayName = "Max")
};