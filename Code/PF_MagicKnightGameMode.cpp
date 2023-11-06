// Copyright Epic Games, Inc. All Rights Reserved.

#include "PF_MagicKnightGameMode.h"
#include "PF_MagicKnightCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

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

	if (IsValid(PlayerWidgetClass))
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetClass);

		if (IsValid(CurrentWidget))
		{
			CurrentWidget->AddToViewport();
		}
	}
}
