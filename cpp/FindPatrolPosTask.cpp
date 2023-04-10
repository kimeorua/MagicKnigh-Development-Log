// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPatrolPosTask.h"
#include "EnemyController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

UFindPatrolPosTask::UFindPatrolPosTask(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("FindPatrolPosTask");
}

EBTNodeResult::Type UFindPatrolPosTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Controller = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	auto Enemy = Controller->GetPawn();

	if (nullptr == Enemy) 
	{
		return EBTNodeResult::Failed;
	}
	//���� �����Ϳ� ������ navi mesh�� �ʱ�ȭ
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (nullptr == NavSystem) 
	{
		//navi mesh�� ���ٸ� ���и� ��ȯ
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Enemy in Navi"));
		return EBTNodeResult::Failed;
	}

	FVector const Origin = Enemy->GetActorLocation();
	FNavLocation NextPatrol;

	//NextPatrol������ ������ location �����͸� �ְ� �ٽ� TargetLocationŰ�� value�� ���� �־��ش�.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, NextPatrol))
	{
		Controller->GetBlackboardComponent()->SetValueAsVector(AEnemyController::TargetLocation, NextPatrol.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
