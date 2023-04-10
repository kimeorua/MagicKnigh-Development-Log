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
	//현재 에디터에 설정된 navi mesh로 초기화
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (nullptr == NavSystem) 
	{
		//navi mesh가 없다면 실패를 반환
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Enemy in Navi"));
		return EBTNodeResult::Failed;
	}

	FVector const Origin = Enemy->GetActorLocation();
	FNavLocation NextPatrol;

	//NextPatrol변수에 임의의 location 데이터를 넣고 다시 TargetLocation키의 value에 값을 넣어준다.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, NextPatrol))
	{
		Controller->GetBlackboardComponent()->SetValueAsVector(AEnemyController::TargetLocation, NextPatrol.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
