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
const FName AEnemyAIController::CanSeePlayer(TEXT("CanSeePlayer")); //플레이어 인식 여부
const FName AEnemyAIController::Player(TEXT("Player"));	//인식한 플레이어 캐릭터
const FName AEnemyAIController::SelfActor(TEXT("SelfActor")); //적 캐릭터 자신
const FName AEnemyAIController::TargetLocation(TEXT("TargetLocation"));	//이동할 좌표
const FName AEnemyAIController::PatrolNum(TEXT("PatrolNum")); //순찰할 번호
const FName AEnemyAIController::EnemyPatternNum(TEXT("EnemyPatternNum")); //랜덤 패턴 번호
const FName AEnemyAIController::Distance(TEXT("Distance")); //플레이어와 적 캐릭터의 거리
const FName AEnemyAIController::IsDie(TEXT("IsDie")); //현재 죽었는지 판단


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
		GetBlackboardComponent()->SetValueAsBool(IsDie, false);
	}
}

//빙의시 호출
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

