// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MiddleRange_Attack.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.h"


UBTT_MiddleRange_Attack::UBTT_MiddleRange_Attack()
{
	NodeName = TEXT("Middle_Range_Attack");
}

EBTNodeResult::Type UBTT_MiddleRange_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Enemy.Pettern.JumpAttack")));
		
		Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);

		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }
}
