### AI 중거리 전투 패턴 및 플레이어 추적 구현
+ #### EEnemyPettern 열거형을 만들어서, 블루프린트 및 C++에서 사용할수 있도로함.
+ #### EEnemyPettern 자료형을 가진 Blackbord 키 값을 설정하고 EnemyAIController에서 접근 할수 있도록 설정 함.
+ #### RandomPettern_MiddleRange Task를 통해 난수를 설정하여 60%의 확률로 추적을 40%의 확률로 공격 패턴에 들어가도록 구현함.

#### EEnemyPattern열거형
```cpp
UENUM(BlueprintType)
enum class EEnemyPattern : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Chase UMETA(DisplayName = "Chase"), //추적
	MiddleRange UMETA(DisplayName = "MiddleRange"), //중거리
	CloseRange UMETA(DisplayName = "CloseRange"), //근거리

	Max UMETA(DisplayName = "Max")
};
```
#### RandomPettern_MiddleRange.cpp

```cpp

UBTT_RandomPettern_MeddleRange::UBTT_RandomPettern_MeddleRange()
{
	NodeName = TEXT("RandomPettern_MeddleRange"); //에디터상에서 표기될 노드 이름
}

EBTNodeResult::Type UBTT_RandomPettern_MeddleRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Enemy))
	{
		int x = 0;	//랜덤한 숫자를 저장할 변수
		x = FMath::RandRange(1, 100);		// 랜덤 값 저장


		if (x <= 60) //60%의 확률
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyPattern, uint8(EEnemyPattern::Chase)); //블랙보드에 패턴을 추적으로 기입
		}
		else if (x <= 100)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyPattern, uint8(EEnemyPattern::MiddleRange)); //블랙보드에 패턴을 중거리 공격으로 기입
		}

		return EBTNodeResult::Succeeded;
	}
	else { return EBTNodeResult::Failed; }
}
```

#### AI비헤이비어트리
![](https://github.com/kimeorua/portfolio/blob/main/img/AI%EC%A0%84%ED%88%AC%EB%B9%84%ED%97%A4%EC%9D%B4%EB%B9%84%EC%96%B4%ED%8A%B8%EB%A6%AC.PNG?raw=true)

+ #### 플레이어가 탐지되면, 플레이와의 거리를 기준으로 패턴을 나눔
+ #### 150 < Distance < 500 이면, RandomPettern_MiddleRange Task를 통하여, 패턴을 정하고, 해당패턴에 맞게 추적 또는 공격에 들어가도록 구성함. 
