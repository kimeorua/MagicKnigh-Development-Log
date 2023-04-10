// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

private:

	//애님 인스턴스
	class UEnemyAnimInstance* EnemyAnimInstance;

	//------------------------Function-----------------------//

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AEnemy();
};
