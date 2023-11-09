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

	// 플레이어 체력
	UPROPERTY()
	float HP;

	// 플레이어 원소력
	UPROPERTY()
	float EF;

	// 플레이어 체간
	UPROPERTY()
	float Posture;

	// 플레이어 회복 횟수
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
