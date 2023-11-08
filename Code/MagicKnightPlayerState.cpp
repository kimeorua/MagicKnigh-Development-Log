// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicKnightPlayerState.h"
#include "MagicKnightSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

AMagicKnightPlayerState::AMagicKnightPlayerState()
{
	//SaveSlotName = "Save1";
}

void AMagicKnightPlayerState::LoadPlayerData(class APlayerCharacter* Player)
{
	UMagicKnightSaveGame* SaveGame = Cast<UMagicKnightSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveState", 0));
	if (!(IsValid(SaveGame)))
	{
		SaveGame = GetMutableDefault<UMagicKnightSaveGame>();
	}
	if (Player != nullptr)
	{
		// Player가 유효하면 저장된 플레이어 스텟을 적용 함.
		Player->GetMagicKnightAttributeSet()->SetHealth(SaveGame->HP);
		Player->GetMagicKnightAttributeSet()->SetElementalForce(SaveGame->EF);
		Player->GetMagicKnightAttributeSet()->SetPosture(SaveGame->Posture);
		Player->GetMagicKnightAttributeSet()->SetHealingCount(SaveGame->HealCount);
	}
}

void AMagicKnightPlayerState::SavePlayerData(float hp, float ef, float posture, float healCount)
{
	UMagicKnightSaveGame* NewGameData = NewObject<UMagicKnightSaveGame>();

	//UE_LOG(LogTemp, Warning, TEXT("SaveGame"));

	// 플레이어가 저장을 사용 하면, 각 스텟의 수치를 저장 함,
	NewGameData->HP = hp;
	NewGameData->EF = ef;
	NewGameData->Posture = posture;
	NewGameData->HealCount = healCount;

	if (UGameplayStatics::SaveGameToSlot(NewGameData, "SaveState", 0) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGame Error!"));
	}
}

void AMagicKnightPlayerState::SaveLevelData(UWorld* CurrentLevel)
{
	UMagicKnightSaveGame* NewLevelData = NewObject<UMagicKnightSaveGame>();
	NewLevelData->Level = CurrentLevel;

	if (UGameplayStatics::SaveGameToSlot(NewLevelData, "SaveLevel", 0) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGame Error!"));
	}
}

TSoftObjectPtr<UWorld> AMagicKnightPlayerState::LoadLevelData()
{
	UMagicKnightSaveGame* SaveGame = Cast<UMagicKnightSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveLevel", 0));

	if (!(IsValid(SaveGame)))
	{
		SaveGame = GetMutableDefault<UMagicKnightSaveGame>();	
	}
	return SaveGame->Level;
}

void AMagicKnightPlayerState::SaveTransfromData(FTransform CurrentTransfrom)
{
	UMagicKnightSaveGame* NewlData = NewObject<UMagicKnightSaveGame>();
	NewlData->Transform = CurrentTransfrom;

	if (UGameplayStatics::SaveGameToSlot(NewlData, "SaveTransfrom", 0) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGame Error!"));
	}
}

void AMagicKnightPlayerState::LoadTransfromData(class APlayerCharacter* Player)
{
	UMagicKnightSaveGame* SaveGame = Cast<UMagicKnightSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveTransfrom", 0));

	if (!(IsValid(SaveGame)))
	{
		SaveGame = GetMutableDefault<UMagicKnightSaveGame>();
	}
	if (Player)
	{
		Player->SetActorTransform(SaveGame->Transform);
	}

}

void AMagicKnightPlayerState::SaveKillEnemyArr(TArray<FName> NewArr)
{
	UMagicKnightSaveGame* NewlData = NewObject<UMagicKnightSaveGame>();
	//NewlData->KillArr.Empty();

	for (FName NewID : NewArr)
	{
		NewlData->KillArr.Add(NewID);
	}
	if (UGameplayStatics::SaveGameToSlot(NewlData, "SaveKillEnemy", 0) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGame Error!"));
	}
}

void AMagicKnightPlayerState::LoadKillEnemyArr(APlayerCharacter* Player)
{
	UMagicKnightSaveGame* SaveGame = Cast<UMagicKnightSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveKillEnemy", 0));

	if (!(IsValid(SaveGame)))
	{
		SaveGame = GetMutableDefault<UMagicKnightSaveGame>();
	}
	if (Player)
	{
		Player->SetKillArry(SaveGame->KillArr);
	}

}

void AMagicKnightPlayerState::SaveReset()
{
	UGameplayStatics::DeleteGameInSlot("SaveState", 0);
	UGameplayStatics::DeleteGameInSlot("SaveLevel", 0);
	UGameplayStatics::DeleteGameInSlot("SaveTransfrom", 0);
	UGameplayStatics::DeleteGameInSlot("SaveKillEnemy", 0);
}
