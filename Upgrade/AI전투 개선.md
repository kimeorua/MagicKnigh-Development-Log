### AI 전투 개선

![](https://github.com/kimeorua/portfolio/blob/main/img/AI%EC%A0%84%ED%88%AC%EA%B0%9C%EC%84%A02.PNG?raw=true)
+ #### 기존의 방식에서 세분화 하여 AI 전투를 개선함.
+ #### InCloseRange 데코레이터를 작성하여 플레이어와 적 캐릭터의 거리가 150 이하인지 판단함.
+ #### 제일 처음으로 근거리(150)인지 판단하고, 아니면, 적과 플레이어 사이의 거리를 통해, 거리가 500보다 크면 추적 패턴, 500 이하면 랜덤 숫자를 뽑아서 20%의 확률로 점프 공격을, 80%의 확률로 추적을 실행 함.

### InCloseAttackRange

```cpp
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_InCloseAttackRange.h"
#include "PlayerCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_InCloseAttackRange::UBTD_InCloseAttackRange()
{
	NodeName = TEXT("In Close Attack Range");
}

bool UBTD_InCloseAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()); // 조종하고있는 폰 정보 변수에 저장
	if (ControllingPawn == nullptr) { return false; }

	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player)); //타겟 변수에 Player저장
	if (Target == nullptr) { return false; }


	bResult = (Target->GetDistanceTo(ControllingPawn) <= 150.0f); // 현제 추적중인 적과의 거리가 150(1.5m)가 되면 true, 아니면 false반환
	return bResult;
}
```
