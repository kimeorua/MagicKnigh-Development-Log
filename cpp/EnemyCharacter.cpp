// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TargetPoint.h"

AEnemyCharacter::AEnemyCharacter()
{
	Controller = nullptr;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	Controller = Cast<AEnemyAIController>(GetController());
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	StartPoint = GetActorLocation();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::FindPlayer()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AEnemyCharacter::LosePlayer()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

TArray<FVector> AEnemyCharacter::GetPatrolPoints() const
{
	TArray<FVector>PatrolPointsArr;
	for (ATargetPoint* Point : PatrolPoints)
	{
		PatrolPointsArr.Add(Point->GetActorLocation());
	}

	return PatrolPointsArr;
}

