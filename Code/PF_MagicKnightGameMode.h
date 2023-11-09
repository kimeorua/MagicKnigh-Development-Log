// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PF_MagicKnightGameMode.generated.h"

UCLASS(minimalapi)
class APF_MagicKnightGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	//-------------UI----------------//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> PlayerWidgetClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;
	//-------------UI----------------//

public:
	APF_MagicKnightGameMode();
	void StartPlay() override;

	UFUNCTION(BlueprintCallable)
	void ShowUI();

	/// <summary>
	/// 처치한 적 저장
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SaveKillEnemyData();

	/// <summary>
	/// 처치한 적 불러오기
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void LoadKillEnemyData();

	/// <summary>
	/// 게임 파일 삭제하여 리셋
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ReSet();

	/// <summary>
	/// 무기 해금 정보 저장
	/// </summary>
	void SaveWeaponData();

	/// <summary>
	/// 무기 해금 정보 로드
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void LoadWeaponData();
};



