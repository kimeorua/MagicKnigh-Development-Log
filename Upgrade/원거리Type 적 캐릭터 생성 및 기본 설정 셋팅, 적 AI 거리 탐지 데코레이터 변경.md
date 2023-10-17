#### 원거리Type 적 캐릭터 생성 및 기본 설정 셋팅
+ #### 원거리Type의 적 캐릭터Blueprint를 생성 하고, 기본적인 GameplayEffect 및 퍼셉션, UI 애니메이션을 할당함.
+ #### 애니메이션 블루 프린트에서 플레이어가 탐지 되면, 대기자세에서 조준 자세로 변경함.

#### 원거리Type 적
![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EC%9B%90%EA%B1%B0%EB%A6%AC%EC%A0%81.PNG?raw=true)

#### 원거리Type 적 애니메이션 블루프린트
![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EC%9B%90%EA%B1%B0%EB%A6%ACABP.PNG?raw=true)

#### 적 AI 거리 탐지 데코레이터 변경
+ #### 기존에 있던 InCloseAttackRange데코레이터를 CheackAttackRange로 변경하고, 탐지 거리를 변수하여, 비헤이비어 트리에서 직접 설정할수 있도록 변경함.
+ #### 이로 인하여, 기존에 근거리(150)뿐만 아니라 유동적으로 거리를 판단 할수 있게 됨.

#### CheackAttackRange::CalculateRawConditionValue()
```cpp
bool UBTD_CheackAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()); // 조종하고있는 폰 정보 변수에 저장
	if (ControllingPawn == nullptr) { return false; }

	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	if (Target == nullptr) { return false; }


	bResult = (Target->GetDistanceTo(ControllingPawn) <= CheackRange); // 현제 추적중인 적과의 거리가 CheackRange보다 작으면 true, 아니면 false반환
	return bResult;
}
```
