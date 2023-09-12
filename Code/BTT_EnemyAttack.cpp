// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"

UBTT_EnemyAttack::UBTT_EnemyAttack()
{
	bNotifyTick = true;
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
		Enemy->GetAbilitySystemComponent()->TryActivateAbilityByClass(AttackAbility);
		bIsAttack = true;
		UE_LOG(LogTemp, Warning, TEXT("True"));

		Enemy->OnAttackEnd.AddLambda([this]() -> void { bIsAttack = false; });
		return EBTNodeResult::InProgress;
	}
	else { return EBTNodeResult::Failed; }
}

void UBTT_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!bIsAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("False"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
