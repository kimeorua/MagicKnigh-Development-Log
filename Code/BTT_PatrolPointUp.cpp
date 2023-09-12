// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PatrolPointUp.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.h"

UBTT_PatrolPointUp::UBTT_PatrolPointUp()
{
	NodeName = TEXT("Patrol Point Up");
}

EBTNodeResult::Type UBTT_PatrolPointUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		int CurrentPoint = OwnerComp.GetBlackboardComponent()->GetValueAsInt(AEnemyAIController::PatrolNum); //현재 패트롤 번호 저장
		if (CurrentPoint == Enemy->GetPatrolPoint().Num() - 1) //현재 패트롤 번호가 마지막 번호이면
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::PatrolNum, 0); //패트롤 번호 초기화
		}
		else if (CurrentPoint != Enemy->GetPatrolPoint().Num() - 1) //마지막 번호가 아니면
		{
			CurrentPoint += 1; //패트롤 번호 증가
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::PatrolNum, CurrentPoint); //해당 번호를 블랙보드에 기입
		}
		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }

}
