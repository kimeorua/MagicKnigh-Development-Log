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
	static void SavePlayerData(float hp, float ef, float posture);

	FString SaveSlotName;
};
