// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheakRange.h"
#include "PlayerCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CheakRange::UBTD_CheakRange()
{
	NodeName = TEXT("CheackInAttackRange");
}

bool UBTD_CheakRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AEnemyCharacter* EnemySelf = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()); // �����ϰ��ִ� �� ���� ������ ����
	if (EnemySelf == nullptr) return false;

	//���� �������� �÷��̾� ������ ����
	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	if (Target == nullptr) return false;


	bResult = (Target->GetDistanceTo(EnemySelf) <= Distance); // ���� �������� ������ �Ÿ��� �����ͻ󿡼� ���� �Ÿ����� ������ true ��ȯ

	return bResult;
}
