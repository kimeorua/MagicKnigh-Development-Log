// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

//무기 Enum
UENUM(BlueprintType)
enum class EEquipedWeapon : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Sword UMETA(DisplayName = "Sword"), //검
	DoubleAxe UMETA(DisplayName = "DoubleAxe"), //쌍수 도끼

	Max UMETA(DisplayName = "Max")
};

//공격 Type
UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"), //기본 근접 공격
	AOE UMETA(DisplayName = "AOE"), //범위 공격
	AOE_Object_Center UMETA(DisplayName = "AOE Object Center"), //특정 객체를 중심으로 범위 공격 (EX: 플레이어의 특수스킬)
	Ramge_Line UMETA(DisplayName = "Range"),

	Max UMETA(DisplayName = "Max")
};

//전투용 Effect Type -> 코드를 읽기쉽게 하기위하여 정의함.
UENUM(BlueprintType)
enum class ECombetEffectType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Block UMETA(DisplayName = "Block"), //가드 성공
	Parrying UMETA(DisplayName = "Parrying"), // 튕겨내기 성공
	Charge UMETA(DisplayName = "EF Charge"), //EF 충전
	DecreasePosture UMETA(DisplayName = "Decrease Posture"), //체간 감소

	Max UMETA(DisplayName = "Max")
};

//데미지 Effect Type -> 코드를 읽기쉽게 하기위하여 정의함.
UENUM(BlueprintType)
enum class EDamageEffectType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	NormalAttack UMETA(DisplayName = "Normal Attack"), //중간 데미지
	LightAttack UMETA(DisplayName = "Light Attack"), //낮은 데미지
	HeavyAttack UMETA(DisplayName = "Heavy Attack"), //높은 데미지
	SpecialAttack UMETA(DisplayName = "Special Attack"), //특수 공격 데미지
	PostureUp UMETA(DisplayName = "Posture Up"), //체간 상승(일반)
	PostureUp_OnParry UMETA(DisplayName = "Posture Up_OnParry"), //체간 상승(튕겨내기 성공 시)

	Max UMETA(DisplayName = "Max")
};