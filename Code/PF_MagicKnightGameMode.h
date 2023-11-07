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
};



