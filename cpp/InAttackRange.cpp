// Fill out your copyright notice in the Description page of Project Settings.


#include "InAttackRange.h"
#include "EnemyController.h"
#include "Enemy.h"
#include "MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UInAttackRange::UInAttackRange()
{
	NodeName = TEXT("InAttackRange"); //표시되는 노드 이름
	Interval = 0.05f; //반복 주기
}

void UInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // ControllingPawn에 현제 조종하는 Pawn 저장
	if (ControllingPawn == nullptr)
	{
		return;
	}
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation(); //후에 사용할OverlapMultiByChannel에서 사용할 중점을 적 캐릭터의 좌표로 설정
	float AttackRange = 300.0f;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults; //오버랩 결과
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel
	(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRange),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const OverlapResult : OverlapResults)
		{
			AMainCharacter* Main = Cast<AMainCharacter>(OverlapResult.GetActor());
			AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
			if (Main && Main->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyController::IsInAttackRange, true);
				return;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Out"));
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyController::IsInAttackRange, false);
	}
	DrawDebugSphere(World, Center, AttackRange, 16, FColor::Red, false, 0.2f);
}
