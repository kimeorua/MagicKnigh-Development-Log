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

	/// <summary>
	/// ���� ����
	/// </summary>
	/// <param name="CurrentLevel">���� ����</param>
	static void SaveLevelData(UWorld* CurrentLevel);

	/// <summary>
	/// ���� �ε�
	/// </summary>
	/// <returns>����� ���� ��ȯ</returns>
	static TSoftObjectPtr<UWorld> LoadLevelData();

	/// <summary>
	/// ���� �÷��̾� ��ġ ����
	/// </summary>
	/// <param name="CurrentTransfrom">���� ��ġ</param>
	static void SaveTransfromData(FTransform CurrentTransfrom);

	/// <summary>
	/// �÷��̾� ��ġ �ε�
	/// </summary>
	/// <param name="Player">�÷��̾�</param>
	static void LoadTransfromData(class APlayerCharacter* Player);

	/// <summary>
	/// ���� �� ĳ����ID����
	/// </summary>
	/// <param name="NewArr">ID �迭</param>
	static void SaveKillEnemyArr(TArray<FName> NewArr);

	/// <summary>
	/// óġ�� �� ID �迭 �ε�
	/// </summary>
	/// <param name="Player">�÷��̾�</param>
	static void LoadKillEnemyArr(class APlayerCharacter* Player);

	/// <summary>
	/// ����� ���� ����
	/// </summary>
	static void SaveReset();

	/// <summary>
	/// ���� �ر� ���� ����
	/// </summary>
	/// <param name="UseAxe">���� �ر� ����</param>
	static void SaveUseAxe(bool UseAxe);

	/// <summary>
	/// ���� �ر� ���� �ε�
	/// </summary>
	/// <param name="Player">�÷��̾�</param>
	static void LoadUseAxe(class APlayerCharacter* Player);
};