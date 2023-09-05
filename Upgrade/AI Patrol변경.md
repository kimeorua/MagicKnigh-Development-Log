### 기존의 무작위 위치로 이동하는 것이 아닌 TargetPoint를 이용하여, 일정 구역을 반복해서 순찰하는 AI 구현

#### 변경된 비헤이비어 트리
![](https://github.com/kimeorua/portfolio/blob/main/img/AIPatrol%EA%B0%9C%EC%84%A0.PNG?raw=true)

#### UBTT_PatrolPointUp.h(순찰 지역 변경)

```cpp

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PatrolPointUp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PF_MAGICKNIGHT_API UBTT_PatrolPointUp : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_PatrolPointUp();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
```

#### UBTT_PatrolPointUp.cpp
```cpp
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PatrolPointUp.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.h"

UBTT_PatrolPointUp::UBTT_PatrolPointUp()
{
	NodeName = TEXT("Patrol Point Up");
}

EBTNodeResult::Type UBTT_PatrolPointUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		int CurrentPoint = OwnerComp.GetBlackboardComponent()->GetValueAsInt(AEnemyAIController::PatrolNum); //현제 번호를 Blackboard에서 가져옴
		if (CurrentPoint == Enemy->GetPatrolPoint().Num() - 1) //현제 번호가 마지막 변호이면
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::PatrolNum, 0); //맨 처음 위치로 이동
		}
		else if (CurrentPoint != Enemy->GetPatrolPoint().Num() - 1) //아니면
		{
			CurrentPoint += 1; 
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::PatrolNum, CurrentPoint);//다음 위치로 이동
		}
		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }

}
```
#### 배열을 이용하여 배열의 Index값을 기준으로, 현제 Point와 다음 Point를 계산하여 Blackboard에 기입 한다.

#### UBTT_SettingPatrolPoint.h
```cpp
	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SettingPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PF_MAGICKNIGHT_API UBTT_SettingPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SettingPatrolPoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
```
#### UBTT_SettingPatrolPoint.cpp

```cpp
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SettingPatrolPoint.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.h"

UBTT_SettingPatrolPoint::UBTT_SettingPatrolPoint()
{
	NodeName = TEXT("Setting Partrol Point");
}

EBTNodeResult::Type UBTT_SettingPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		ATargetPoint* Point =  Enemy->GetPatrolPoint()[OwnerComp.GetBlackboardComponent()->GetValueAsInt(AEnemyAIController::PatrolNum)];
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::TargetLocation, Point->GetActorLocation());

		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }
}
```

#### UBTT_SettingPatrolPoint()함수는 실제로 이동할 좌표를 구하는 함수로 UBTT_SettingPatrolPoint()함수에서 블랙보드에 기입된 Point 번호를 배열에서 찾아, 해당 Point의 좌표를 Blackborad에 기입 한다.
