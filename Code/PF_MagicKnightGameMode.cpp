// Copyright Epic Games, Inc. All Rights Reserved.

#include "PF_MagicKnightGameMode.h"
#include "PF_MagicKnightCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MagicKnightPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

APF_MagicKnightGameMode::APF_MagicKnightGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
	
}

void APF_MagicKnightGameMode::StartPlay()
{
	Super::StartPlay();

	
}

void APF_MagicKnightGameMode::ShowUI()
{
	if (IsValid(PlayerWidgetClass))
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetClass);
	}
	if (IsValid(CurrentWidget))
	{
		CurrentWidget->AddToViewport();
	}
}

void APF_MagicKnightGameMode::SaveKillEnemyData()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Player))
	{
		AMagicKnightPlayerState::SaveKillEnemyArr(Player->GetKillEnemyID_Arr());
	}
}

void APF_MagicKnightGameMode::LoadKillEnemyData()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Player))
	{
		AMagicKnightPlayerState::LoadKillEnemyArr(Player);
	}
}

void APF_MagicKnightGameMode::ReSet()
{
	AMagicKnightPlayerState::SaveReset();
}
