// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameData.h"
#include "CharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCharacterGameplayAbility();
	//���콺&Ű���� �Է� ����
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EGameAbilityInputID AbilityInputID  = EGameAbilityInputID::None;
};
