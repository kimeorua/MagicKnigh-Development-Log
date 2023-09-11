// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_DistanceToPlayer.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"

UBTS_DistanceToPlayer::UBTS_DistanceToPlayer()
{
	NodeName = TEXT("DistanceToPlayer");
	Interval = 0.1f;
}

void UBTS_DistanceToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyCharacter* EnemySelf = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));

	if (EnemySelf == nullptr) return;
	if (Player == nullptr) return;

	Distance = Player->GetHorizontalDistanceTo(EnemySelf);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AEnemyAIController::Distance, Distance);
}
