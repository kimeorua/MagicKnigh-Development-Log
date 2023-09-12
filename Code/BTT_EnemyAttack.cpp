// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"

UBTT_EnemyAttack::UBTT_EnemyAttack()
{
	bNotifyTick = true; //TickTask ���
	AttackAbility = nullptr;
	NodeName = TEXT("Enemy Attack");
	bIsAttack = false;
}

EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		Enemy->GetAbilitySystemComponent()->TryActivateAbilityByClass(AttackAbility); //�����Ƽ �۵�
		bIsAttack = true;

		Enemy->OnAttackEnd.AddLambda([this]() -> void { bIsAttack = false; }); //���ٽ�����, OnAttackEnd��������Ʈ �ڵ� ����
		return EBTNodeResult::InProgress; //Task ���������� ����
	}
	else { return EBTNodeResult::Failed; }
}

void UBTT_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!bIsAttack)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //Task �������� ����
	}
}
