### AI 공격 완성
+ #### BTT_EnemyAttack 작성 및, 중거리 패턴에 적용
+ #### 델리게이트를 이용하여, 애니메이션 몽타주가 끝나면 함수를 호출하여, Task를 중지 함.

#### AEnemyCharacter.h

```cpp
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate); //델리게이트 선언
public:
	FOnAttackEndDelegate OnAttackEnd;

protected:
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY()
	class UEnemyAnimInstance* EnemyAnim; //애니메이션 호출을 위한 변수

  UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted); // 몽타주가 끝나면 호출되는 OnMontageEnded 델리게이트에서 호출될 함수
```

#### AEnemyCharacter.cpp
```cpp
void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	EnemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance()); //애니인스턴스 저장
	EnemyAnim->OnMontageEnded.AddDynamic(this, &AEnemyCharacter::OnAttackMontageEnded);
}

void AEnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast(); //BTTask_EnemyAttack에서 선언한 공격 종료 함수가 호출되도록 델리게이트를 이용하여 구축
}
```


#### BTT_EnemyAttack
```cpp
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"

UBTT_EnemyAttack::UBTT_EnemyAttack()
{
	bNotifyTick = true; // TickTask 사용 가능 하도록 변경
	AttackAbility = nullptr; //사용할 공격 어빌리티 (에디터에서 설정)
	NodeName = TEXT("Enemy Attack"); //표시될 노드 이름
	bIsAttack = false; //공격 여부
}

EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		Enemy->GetAbilitySystemComponent()->TryActivateAbilityByClass(AttackAbility); //어빌리티 작동
		bIsAttack = true;          // 현제 공격중으로 변경

		Enemy->OnAttackEnd.AddLambda([this]() -> void { bIsAttack = false; }); //몽타주가 끝나면 람다식으로 해당 함수를 작동
		return EBTNodeResult::InProgress; //현제 Task진행중
	}
	else { return EBTNodeResult::Failed; }
}

void UBTT_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!bIsAttack) //공격 중이 아니면
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //Task 완료로 변경
	}
}

```

#### 비헤이비어 트리
![](https://github.com/kimeorua/portfolio/blob/main/img/AI%EA%B3%B5%EA%B2%A9.PNG?raw=true)
