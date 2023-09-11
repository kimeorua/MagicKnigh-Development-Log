// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_InCloseAttackRange.h"
#include "PlayerCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_InCloseAttackRange::UBTD_InCloseAttackRange()
{
	NodeName = TEXT("In Close Attack Range");
}

bool UBTD_InCloseAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()); // 조종하고있는 폰 정보 변수에 저장
	if (ControllingPawn == nullptr) { return false; }

	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	if (Target == nullptr) { return false; }


	bResult = (Target->GetDistanceTo(ControllingPawn) <= 150.0f); // 현제 추적중인 적과의 거리가 150(1.5m)가 되면 true, 아니면 false반환
	return bResult;
}
