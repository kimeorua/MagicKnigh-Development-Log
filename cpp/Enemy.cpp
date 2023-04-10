// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAnimInstance.h"

AEnemy::AEnemy()
{
	EnemyAnimInstance = nullptr;
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

