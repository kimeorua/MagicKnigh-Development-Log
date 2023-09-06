// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RandomPettern_MeddleRange.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.h"
#include "MagicKnightEnums.h"

UBTT_RandomPettern_MeddleRange::UBTT_RandomPettern_MeddleRange()
{
	NodeName = TEXT("RandomPettern_MeddleRange");
}

EBTNodeResult::Type UBTT_RandomPettern_MeddleRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		int x = 0;	//랜덤한 숫자를 저장할 변수
		x = FMath::RandRange(1, 100);		// 랜덤 값 저장


		if (x <= 60)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyPattern, uint8(EEnemyPattern::Chase));
		}
		else if (x <= 100)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyPattern, uint8(EEnemyPattern::MiddleRange));
		}

		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }
}
