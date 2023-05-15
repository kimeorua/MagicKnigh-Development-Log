// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

//접근할 이름에 블랙보드 키값 할당
const FName AEnemyController::TargetLocation(TEXT("TargetLocation"));
const FName AEnemyController::CanSeePlayer(TEXT("CanSeePlayer"));
const FName AEnemyController::Player(TEXT("Player"));
const FName AEnemyController::IsInAttackRange(TEXT("IsInAttackRange"));
const FName AEnemyController::AttackPattern(TEXT("AttackPattern"));

AEnemyController::AEnemyController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	SetPerceptionSystem();
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (IsValid(Blackboard) && IsValid(BehaviorTree))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset); //블랙보드 키값 초기화 실시
	}
}

void AEnemyController::OnUpdated(TArray<AActor*> const& updated_actors)
{
	UE_LOG(LogTemp, Warning, TEXT("Updating"));
	
}

void AEnemyController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (auto const PlayerPawn = Cast<AMainCharacter>(actor))
	{
		//성공적으로 플레이어를 감지 하면 true값을 넣어준다.
		GetBlackboardComponent()->SetValueAsBool(CanSeePlayer, Stimulus.WasSuccessfullySensed());
		
		//CanSeePlayer가 true면 플레이어를 감지 한것 이고 false면 감지 하지 못한 것으로 판단함. 
		if (GetBlackboardComponent()->GetValueAsBool(CanSeePlayer))
		{
			// Player 에 플레이어 캐릭터 저장 및 이동 속도 증가
			GetBlackboardComponent()->SetValueAsObject(Player, PlayerPawn);
			AEnemy* Enemy = Cast<AEnemy>(GetPawn());
			Enemy->GetCharacterMovement()->MaxWalkSpeed = 550.f;

			AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			SetFocus(Main);
		}
		else
		{
			// 플레이어가 감지 범위 밖으로 나갔거나, 감지 하지 않은 것이므로 Player에 nullptr저장 및 이동속도 감소
			GetBlackboardComponent()->SetValueAsObject(Player, nullptr);
			GetBlackboardComponent()->SetValueAsBool(IsInAttackRange, false);
			AEnemy* Enemy = Cast<AEnemy>(GetPawn());
			Enemy->GetCharacterMovement()->MaxWalkSpeed = 300.f;

			AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}

//PerceptionComponent 초기화 담당 함수
void AEnemyController::SetPerceptionSystem()
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
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::OnUpdated);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetDetected);
}