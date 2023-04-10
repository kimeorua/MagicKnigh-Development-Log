// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

// 입력 값에 대한 열거형 = 프로젝트 세팅에 지정한 문자열과 같아야 함
UENUM(BlueprintType)
enum class EGameAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Block,
	SwordEquip,
	AxeEquip,
	QSkill,
	ESkill,
	EPSkill
};
