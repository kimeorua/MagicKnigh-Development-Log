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
	/// 게임 로드
	/// </summary>
	/// <param name="Player">플레이어 캐릭터</param>
	static void LoadPlayerData(class APlayerCharacter* Player);

	/// <summary>
	/// 게임 저장
	/// </summary>
	/// <param name="hp">플레이어 체력</param>
	/// <param name="ef">플레이어 원소력</param>
	/// <param name="posture">플레이어 체간</param>
	static void SavePlayerData(float hp, float ef, float posture, float healCount);

	/// <summary>
	/// 레벨 저장
	/// </summary>
	/// <param name="CurrentLevel">현재 레벨</param>
	static void SaveLevelData(UWorld* CurrentLevel);

	/// <summary>
	/// 레벨 로드
	/// </summary>
	/// <returns>저장된 레벨 반환</returns>
	static TSoftObjectPtr<UWorld> LoadLevelData();

	/// <summary>
	/// 현재 플레이어 위치 저장
	/// </summary>
	/// <param name="CurrentTransfrom">현재 위치</param>
	static void SaveTransfromData(FTransform CurrentTransfrom);

	/// <summary>
	/// 플레이어 위치 로드
	/// </summary>
	/// <param name="Player">플레이어</param>
	static void LoadTransfromData(class APlayerCharacter* Player);

	/// <summary>
	/// 잡은 적 캐릭터ID저장
	/// </summary>
	/// <param name="NewArr">ID 배열</param>
	static void SaveKillEnemyArr(TArray<FName> NewArr);

	/// <summary>
	/// 처치한 적 ID 배열 로드
	/// </summary>
	/// <param name="Player">플레이어</param>
	static void LoadKillEnemyArr(class APlayerCharacter* Player);

	/// <summary>
	/// 저장된 파일 삭제
	/// </summary>
	static void SaveReset();

	/// <summary>
	/// 무기 해금 여부 저장
	/// </summary>
	/// <param name="UseAxe">도끼 해금 여부</param>
	static void SaveUseAxe(bool UseAxe);

	/// <summary>
	/// 도끼 해금 여부 로드
	/// </summary>
	/// <param name="Player">플레이어</param>
	static void LoadUseAxe(class APlayerCharacter* Player);
};