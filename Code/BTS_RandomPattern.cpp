// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RandomPattern.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_RandomPattern::UBTS_RandomPattern()
{
	NodeName = (TEXT("RandomAttack")); //ǥ�õ� ��� �̸�
	Interval = 1.0f;											// �ݺ��Ǵ� �ð�
	bNotifyBecomeRelevant = true;				//OnBecomeRelevant() �Լ� ���
}

void UBTS_RandomPattern::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	int x = 0;	//������ ���ڸ� ������ ����
	x = FMath::RandRange(1, 100);		// ���� �� ����

	if (x <= 10)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 1); //10%
	}
	else if (x <= 20)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 2); //10%
	}
	else if (x <= 50)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 3); //30%
	}
	else if (x <= 80)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 4); //30%
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 5); //20%
	}

}