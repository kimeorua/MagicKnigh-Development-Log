### AI 전투 시스템 3차 구현
+ #### 기존 점프공격 BTT(비헤이비어트리 테스크) BTS(비헤이비어트리 서비스)를 1개의 GameplayAbility로 묶어 GA_JumpAttack을 만듬
+ #### C++로 BTT_MiddleRange_Attack 테스크를 작성하여, 해당 태스크에서 GA_JumpAttack Ability를 작동하도록 코드를 작성함.


#### 비헤이비어트리
![](https://github.com/kimeorua/portfolio/blob/main/img/AI%EC%A4%91%EA%B1%B0%EB%A6%AC%ED%8C%A8%ED%84%B4%EC%99%84%EC%84%B1.PNG?raw=true)

#### GA_JumpAttack
![](https://github.com/kimeorua/portfolio/blob/main/img/%EC%A0%90%ED%94%84%EC%96%B4%ED%83%9D%EC%96%B4%EB%B9%8C%EB%A6%AC%ED%8B%B0.PNG?raw=true)
+ #### 해당 Ability는 Enemy.Pettern.JumpAttack Tag를 통해 작동 되며, 작동 시 적 캐릭터에게 Enemy.State.Attack Tag를 부착함.
+ #### 이 Ability 먼저 LaunchCharacter()함수로 적 캐릭터를 날리고, 공격 애니메이션을 작동 하는 형식으로 되어 있음.

#### BTT_MiddleRange_Attack.cpp
```cpp
#include "BTT_MiddleRange_Attack.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.h"


UBTT_MiddleRange_Attack::UBTT_MiddleRange_Attack()
{
	NodeName = TEXT("Middle_Range_Attack"); //에디터에서 표기될 노드 이름
}

EBTNodeResult::Type UBTT_MiddleRange_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		FGameplayTagContainer TagContainer; //TagContainer 선언
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Enemy.Pettern.JumpAttack"))); //TagContainer에 Tag 추가
		
		Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer); //해당 Tag의 GameplayAbility를 작동 시킴

		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }
}
```
