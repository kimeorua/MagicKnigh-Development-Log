// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

// �Է� ���� ���� ������ = ������Ʈ ���ÿ� ������ ���ڿ��� ���ƾ� ��
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
