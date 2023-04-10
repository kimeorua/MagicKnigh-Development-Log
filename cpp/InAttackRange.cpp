// Fill out your copyright notice in the Description page of Project Settings.


#include "InAttackRange.h"
#include "EnemyController.h"
#include "Enemy.h"
#include "MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UInAttackRange::UInAttackRange()
{
	NodeName = TEXT("InAttackRange"); //ǥ�õǴ� ��� �̸�
	Interval = 0.05f; //�ݺ� �ֱ�
}

void UInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // ControllingPawn�� ���� �����ϴ� Pawn ����
	if (ControllingPawn == nullptr)
	{
		return;
	}
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation(); //�Ŀ� �����OverlapMultiByChannel���� ����� ������ �� ĳ������ ��ǥ�� ����
	float AttackRange = 300.0f;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults; //������ ���
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
