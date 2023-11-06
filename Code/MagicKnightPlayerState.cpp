// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicKnightPlayerState.h"
#include "MagicKnightSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

AMagicKnightPlayerState::AMagicKnightPlayerState()
{
	SaveSlotName = "Save1";
}

void AMagicKnightPlayerState::LoadPlayerData(class APlayerCharacter* Player)
{
	UMagicKnightSaveGame* SaveGame = Cast<UMagicKnightSaveGame>(UGameplayStatics::LoadGameFromSlot("Save1", 0));
	if (!(IsValid(SaveGame)))
	{
		SaveGame = GetMutableDefault<UMagicKnightSaveGame>();
	}
	if (Player != nullptr)
	{
		// Player�� ��ȿ�ϸ� ����� �÷��̾� ������ ���� ��.
		Player->GetMagicKnightAttributeSet()->SetHealth(SaveGame->HP);
		Player->GetMagicKnightAttributeSet()->SetElementalForce(SaveGame->EF);
		Player->GetMagicKnightAttributeSet()->SetPosture(SaveGame->Posture);
	}
}

void AMagicKnightPlayerState::SavePlayerData(float hp, float ef, float posture)
{
	UMagicKnightSaveGame* NewGameData = NewObject<UMagicKnightSaveGame>();

	UE_LOG(LogTemp, Warning, TEXT("SaveGame"));

	// �÷��̾ ������ ��� �ϸ�, �� ������ ��ġ�� ���� ��,
	NewGameData->HP = hp;
	NewGameData->EF = ef;
	NewGameData->Posture = posture;

	if (UGameplayStatics::SaveGameToSlot(NewGameData, "Save1", 0) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGame Error!"));
	}
}
