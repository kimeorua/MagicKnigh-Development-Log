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

//������ �̸��� ������ Ű�� �Ҵ�
const FName AEnemyAIController::CanSeePlayer(TEXT("CanSeePlayer"));
const FName AEnemyAIController::Player(TEXT("Player"));
const FName AEnemyAIController::SelfActor(TEXT("SelfActor"));
const FName AEnemyAIController::TargetLocation(TEXT("TargetLocation"));
const FName AEnemyAIController::PatrolNum(TEXT("PatrolNum"));
const FName AEnemyAIController::EnemyPatternNum(TEXT("EnemyPatternNum"));
const FName AEnemyAIController::Distance(TEXT("Distance"));
const FName AEnemyAIController::IsDie(TEXT("IsDie"));


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

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (IsValid(Blackboard) && IsValid(BehaviorTree))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset); //������ Ű�� �ʱ�ȭ �ǽ�
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
		//���������� �÷��̾ ���� �ϸ� true���� �־��ش�.
		GetBlackboardComponent()->SetValueAsBool(CanSeePlayer, Stimulus.WasSuccessfullySensed());

		//CanSeePlayer�� true�� �÷��̾ ���� �Ѱ� �̰� false�� ���� ���� ���� ������ �Ǵ���. 
		if (GetBlackboardComponent()->GetValueAsBool(CanSeePlayer))
		{
			//�÷��̾� �߽߰� Enemy.State.FindPlayer�±�  �߰�
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
			//�÷��̾ �νĹ��� ������ ������ Enemy.State.FindPlayer�±�  ����
			if (Enemy->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.FindPlayer"))) > 0)
			{
				Enemy->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Enemy.State.FindPlayer")));
			}

			// �÷��̾ ���� ���� ������ �����ų�, ���� ���� ���� ���̹Ƿ� Player�� nullptr���� �� �̵��ӵ� ����
			GetBlackboardComponent()->SetValueAsObject(Player, nullptr);
			Enemy->LosePlayer();
			UE_LOG(LogTemp, Warning, TEXT("PlayerLose"));

			ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}

void AEnemyAIController::SetPerceptionSystem()
{
	//PerceptionComponent�Ҵ� �� �ֿ� ���� ����
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	//�þ� ����, ������ ���� ���� ����
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	//������ ��ü ����(��, �߸�, �Ʊ�)
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//�ֿ� ���� �ɷ� ����
	PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());

	//�Լ� ���ε�
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnUpdated);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
}

