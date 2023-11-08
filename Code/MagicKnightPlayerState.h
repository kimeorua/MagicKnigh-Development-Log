// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MagicKnightPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API AMagicKnightPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMagicKnightPlayerState();

	/// <summary>
	/// ���� �ε�
	/// </summary>
	/// <param name="Player">�÷��̾� ĳ����</param>
	static void LoadPlayerData(class APlayerCharacter* Player);

	/// <summary>
	/// ���� ����
	/// </summary>
	/// <param name="hp">�÷��̾� ü��</param>
	/// <param name="ef">�÷��̾� ���ҷ�</param>
	/// <param name="posture">�÷��̾� ü��</param>
	static void SavePlayerData(float hp, float ef, float posture, float healCount);

	static void SaveLevelData(UWorld* CurrentLevel);

	static TSoftObjectPtr<UWorld> LoadLevelData();

	static void SaveTransfromData(FTransform CurrentTransfrom);

	static void LoadTransfromData(class APlayerCharacter* Player);

	static void SaveKillEnemyArr(TArray<FName> NewArr);

	static void LoadKillEnemyArr(class APlayerCharacter* Player);

	static void SaveReset();
};