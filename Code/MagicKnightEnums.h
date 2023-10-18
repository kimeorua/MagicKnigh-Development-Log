// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

//���� Enum
UENUM(BlueprintType)
enum class EEquipedWeapon : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Sword UMETA(DisplayName = "Sword"), //��
	DoubleAxe UMETA(DisplayName = "DoubleAxe"), //�ּ� ����

	Max UMETA(DisplayName = "Max")
};

//���� Type
UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"), //�⺻ ���� ����
	AOE UMETA(DisplayName = "AOE"), //���� ����
	AOE_Object_Center UMETA(DisplayName = "AOE Object Center"), //Ư�� ��ü�� �߽����� ���� ���� (EX: �÷��̾��� Ư����ų)
	Ramge_Line UMETA(DisplayName = "Range"),

	Max UMETA(DisplayName = "Max")
};

//������ Effect Type -> �ڵ带 �б⽱�� �ϱ����Ͽ� ������.
UENUM(BlueprintType)
enum class ECombetEffectType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Block UMETA(DisplayName = "Block"), //���� ����
	Parrying UMETA(DisplayName = "Parrying"), // ƨ�ܳ��� ����
	Charge UMETA(DisplayName = "EF Charge"), //EF ����
	DecreasePosture UMETA(DisplayName = "Decrease Posture"), //ü�� ����

	Max UMETA(DisplayName = "Max")
};

//������ Effect Type -> �ڵ带 �б⽱�� �ϱ����Ͽ� ������.
UENUM(BlueprintType)
enum class EDamageEffectType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	NormalAttack UMETA(DisplayName = "Normal Attack"), //�߰� ������
	LightAttack UMETA(DisplayName = "Light Attack"), //���� ������
	HeavyAttack UMETA(DisplayName = "Heavy Attack"), //���� ������
	SpecialAttack UMETA(DisplayName = "Special Attack"), //Ư�� ���� ������
	PostureUp UMETA(DisplayName = "Posture Up"), //ü�� ���(�Ϲ�)
	PostureUp_OnParry UMETA(DisplayName = "Posture Up_OnParry"), //ü�� ���(ƨ�ܳ��� ���� ��)

	Max UMETA(DisplayName = "Max")
};