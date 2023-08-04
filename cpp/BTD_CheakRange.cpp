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

	AEnemyCharacter* EnemySelf = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()); // 조종하고있는 폰 정보 변수에 저장
	if (EnemySelf == nullptr) return false;

	//현재 추적중인 플레이어 변수에 저장
	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	if (Target == nullptr) return false;


	bResult = (Target->GetDistanceTo(EnemySelf) <= Distance); // 현제 추적중인 적과의 거리가 에디터상에서 정한 거리보다 작으면 true 반환

	return bResult;
}
