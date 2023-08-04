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