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
	/// óġ�� �� ����
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SaveKillEnemyData();

	/// <summary>
	/// óġ�� �� �ҷ�����
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void LoadKillEnemyData();

	/// <summary>
	/// ���� ���� �����Ͽ� ����
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ReSet();

	/// <summary>
	/// ���� �ر� ���� ����
	/// </summary>
	void SaveWeaponData();

	/// <summary>
	/// ���� �ر� ���� �ε�
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void LoadWeaponData();
};



