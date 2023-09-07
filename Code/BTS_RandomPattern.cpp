// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RandomPattern.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_RandomPattern::UBTS_RandomPattern()
{
	NodeName = (TEXT("RandomAttack")); //표시될 노드 이름
	Interval = 1.0f;											// 반복되는 시간
	bNotifyBecomeRelevant = true;				//OnBecomeRelevant() 함수 사용
}

void UBTS_RandomPattern::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	int x = 0;	//랜덤한 숫자를 저장할 변수
	x = FMath::RandRange(1, 100);		// 랜덤 값 저장

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