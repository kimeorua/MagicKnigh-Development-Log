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
		int CurrentPoint = OwnerComp.GetBlackboardComponent()->GetValueAsInt(AEnemyAIController::PatrolNum); //���� ��Ʈ�� ��ȣ ����
		if (CurrentPoint == Enemy->GetPatrolPoint().Num() - 1) //���� ��Ʈ�� ��ȣ�� ������ ��ȣ�̸�
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::PatrolNum, 0); //��Ʈ�� ��ȣ �ʱ�ȭ
		}
		else if (CurrentPoint != Enemy->GetPatrolPoint().Num() - 1) //������ ��ȣ�� �ƴϸ�
		{
			CurrentPoint += 1; //��Ʈ�� ��ȣ ����
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::PatrolNum, CurrentPoint); //�ش� ��ȣ�� �����忡 ����
		}
		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }

}
