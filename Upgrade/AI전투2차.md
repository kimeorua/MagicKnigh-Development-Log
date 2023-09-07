### AI 전투: 중거리 패턴(점프 공격) 구현
+ #### 기존의 RandomPettern_MiddleRange를 BTT(비헤이비어 트리 테스크)에서 BTS(비헤이비어 트리 서비스)로 변경
+ #### 서비스로 변경 하면서, 기존의 Enum으로 패턴을 구분하는것이 아닌 숫자를 통해 구분하여, 다른 패턴에서도 사용할수 있도록 개선함.
+ #### 점프공격을 할때 LaunchCharacter() 함수를 통해 일정 힘으로 적 캐릭터를 발사 시켜 공중에 뜨게하고, 공중에 뜨면서 공격 애니메이션을 재생 시킴

#### BTS_RandomPetter.cpp
```cpp
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RandomPattern.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_RandomPattern::UBTS_RandomPattern()
{
	NodeName = (TEXT("RandomAttack")); //표시될 노드 이름
	Interval = 1.0f;											// 반복되는 시간
	bNotifyBecomeRelevant = true;				//OnBecomeRelevant() 함수 사용 -> 보조 노드가 활성화될 때 호출되는 이 함수는 노드가 인스턴스화되지 않은 경우 const로 간주
}

void UBTS_RandomPattern::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	int x = 0;	//랜덤한 숫자를 저장할 변수
	x = FMath::RandRange(1, 100);		// 랜덤 값 저장

	if (x <= 10)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 1); //10%
	}
	else if (x <= 20)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 2); //10%
	}
	else if (x <= 50)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 3); //30%
	}
	else if (x <= 80)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 4); //30%
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::EnemyPatternNum, 5); //20%
	}

}
```

#### 점프 공격 BTT(비헤이비어 트리 테스크) -> 공격 애니메이션 재생
![](https://github.com/kimeorua/portfolio/blob/main/img/BTT_JumpAttack.PNG?raw=true)
#### 점프 공격 BTS(비헤이비어 트리 서비스) -> 애니메이션 재생 중 날라 가는 시스템
![](https://github.com/kimeorua/portfolio/blob/main/img/BTS_JumpAttack.PNG?raw=true)
+ #### 해당 기능들은 후에 GameplayAbility를 응용하여 재 구현할 예정.
