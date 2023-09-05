### 적 캐릭터가 사용할 공통 AI Class를 작성 함.
+ #### AI Perception을 사용하여 시각 인지를 구현 함.

#### AEnemyAIController.h

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAIController.generated.h"

UCLASS()
class PF_MAGICKNIGHT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree; //비헤이비어 트리

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent; //비헤이비어 트리 컴포넌트

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent; //블랙보드 컴포넌트

	class UAISenseConfig_Sight* SightConfig; //시각 인지 컴포넌트

	class AEnemyCharacter* Enemy;

protected:
	void BeginPlay()override;
	virtual void OnPossess(APawn* InPawn) override;     //폰 빙의시 호출 되는 함수


public:
	//-------------------------------------------블랙보드 키에 접근시 사용할 이름들-------------------------------------------//
	static const FName CanSeePlayer;
	static const FName Player;
	static const FName SelfActor;
	static const FName Distance;
	static const FName TargetLocation;
	static const FName PatrolNum;
	//-------------------------------------------블랙보드 키에 접근시 사용할 이름들-------------------------------------------//

	//-------------------------------------------------AI Perception 변수-------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightRadius = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILoseSightRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AIFieldOfView = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightAge = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILastSeenLocation = 100.f;

	//-------------------------------------------------함수-------------------------------------------------//
	AEnemyAIController();
	
	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& updated_actors);
	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	UFUNCTION()
	void SetPerceptionSystem();

	//블랙보드 컴포넌트 반환
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return BehaviorTreeComponent; }
};
```

#### AEnemyAIController.cpp

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerCharacter.h"
#include "GameplayTagContainer.h"
#include "EnemyCharacter.h"

//접근할 이름에 블랙보드 키값 할당
const FName AEnemyAIController::CanSeePlayer(TEXT("CanSeePlayer"));
const FName AEnemyAIController::Player(TEXT("Player"));
const FName AEnemyAIController::SelfActor(TEXT("SelfActor"));
const FName AEnemyAIController::Distance(TEXT("Distance"));
const FName AEnemyAIController::TargetLocation(TEXT("TargetLocation"));
const FName AEnemyAIController::PatrolNum(TEXT("PatrolNum"));


AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	Enemy = nullptr;
	SetPerceptionSystem();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(BehaviorTree))
	{
		BehaviorTreeComponent->StartTree(*BehaviorTree);
		Enemy = Cast<AEnemyCharacter>(GetPawn());
		GetBlackboardComponent()->SetValueAsObject(SelfActor, Enemy);
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (IsValid(Blackboard) && IsValid(BehaviorTree))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset); //블랙보드 키값 초기화 실시
		if (Enemy == nullptr)
		{
			Enemy = Cast<AEnemyCharacter>(GetPawn());
			GetBlackboardComponent()->SetValueAsObject(SelfActor, Enemy);
		}
	}
}

void AEnemyAIController::OnUpdated(TArray<AActor*> const& updated_actors)
{
	UE_LOG(LogTemp, Warning, TEXT("Updating"));
}

void AEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (APlayerCharacter* const PlayerPawn = Cast<APlayerCharacter>(actor))
	{
		//성공적으로 플레이어를 감지 하면 true값을 넣어준다.
		GetBlackboardComponent()->SetValueAsBool(CanSeePlayer, Stimulus.WasSuccessfullySensed());

		//CanSeePlayer가 true면 플레이어를 감지 한것 이고 false면 감지 하지 못한 것으로 판단함. 
		if (GetBlackboardComponent()->GetValueAsBool(CanSeePlayer))
		{
			//플레이어 발견시 Enemy.State.FindPlayer태그  추가
			if (Enemy->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.FindPlayer"))) <= 0)
			{
				Enemy->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Enemy.State.FindPlayer")));
			}
			GetBlackboardComponent()->SetValueAsObject(Player, PlayerPawn);
			Enemy->FindPlayer();
			UE_LOG(LogTemp, Warning, TEXT("PlayerDetected"));

			SetFocus(PlayerPawn);
		}
		else
		{
			//플레이어가 인식범위 밖으로 나가면 Enemy.State.FindPlayer태그  제거
			if (Enemy->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.FindPlayer"))) > 0)
			{
				Enemy->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Enemy.State.FindPlayer")));
			}

			// 플레이어가 감지 범위 밖으로 나갔거나, 감지 하지 않은 것이므로 Player에 nullptr저장 및 이동속도 감소
			GetBlackboardComponent()->SetValueAsObject(Player, nullptr);
			Enemy->LosePlayer();
			UE_LOG(LogTemp, Warning, TEXT("PlayerLose"));

			ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}

void AEnemyAIController::SetPerceptionSystem()
{
	//PerceptionComponent할당 및 주요 감지 설정
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	//시야 범위, 각도등 기초 정보 설정
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	//감지할 객체 정보(적, 중립, 아군)
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//주요 감지 능력 설정
	PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());

	//함수 바인딩
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnUpdated);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
}

```
