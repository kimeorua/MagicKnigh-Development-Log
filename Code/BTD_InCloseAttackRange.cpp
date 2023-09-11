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

	auto ControllingPawn = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()); // �����ϰ��ִ� �� ���� ������ ����
	if (ControllingPawn == nullptr) { return false; }

	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	if (Target == nullptr) { return false; }


	bResult = (Target->GetDistanceTo(ControllingPawn) <= 150.0f); // ���� �������� ������ �Ÿ��� 150(1.5m)�� �Ǹ� true, �ƴϸ� false��ȯ
	return bResult;
}
