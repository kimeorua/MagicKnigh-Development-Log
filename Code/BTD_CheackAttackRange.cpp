// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheackAttackRange.h"
#include "PlayerCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CheackAttackRange::UBTD_CheackAttackRange()
{
	NodeName = TEXT("Cheack Attack Range");
}

bool UBTD_CheackAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()); // �����ϰ��ִ� �� ���� ������ ����
	if (ControllingPawn == nullptr) { return false; }

	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	if (Target == nullptr) { return false; }


	bResult = (Target->GetDistanceTo(ControllingPawn) <= CheackRange); // ���� �������� ������ �Ÿ��� 150(1.5m)�� �Ǹ� true, �ƴϸ� false��ȯ
	return bResult;
}
