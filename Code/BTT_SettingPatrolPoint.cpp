// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SettingPatrolPoint.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.h"

UBTT_SettingPatrolPoint::UBTT_SettingPatrolPoint()
{
	NodeName = TEXT("Setting Partrol Point");
}

EBTNodeResult::Type UBTT_SettingPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		ATargetPoint* Point =  Enemy->GetPatrolPoint()[OwnerComp.GetBlackboardComponent()->GetValueAsInt(AEnemyAIController::PatrolNum)]; //현재 패트롤 번호를 받아서, 배열의 타겟 포인트를 반환
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::TargetLocation, Point->GetActorLocation()); // 해당 타겟 포인트의 위치를 블랙보드에 기입

		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }
}
