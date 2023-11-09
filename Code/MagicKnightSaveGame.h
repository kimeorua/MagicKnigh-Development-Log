// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MagicKnightSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UMagicKnightSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMagicKnightSaveGame();

	// �÷��̾� ü��
	UPROPERTY()
	float HP;

	// �÷��̾� ���ҷ�
	UPROPERTY()
	float EF;

	// �÷��̾� ü��
	UPROPERTY()
	float Posture;

	// �÷��̾� ȸ�� Ƚ��
	UPROPERTY()
	float HealCount;

	UPROPERTY()
	TSoftObjectPtr<UWorld> Level;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<FName> KillArr;

	UPROPERTY()
	bool bCanUseAxe;
};
