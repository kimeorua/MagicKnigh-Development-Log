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

//������ �̸��� ������ Ű�� �Ҵ�
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
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset); //������ Ű�� �ʱ�ȭ �ǽ�
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
		//���������� �÷��̾ ���� �ϸ� true���� �־��ش�.
		GetBlackboardComponent()->SetValueAsBool(CanSeePlayer, Stimulus.WasSuccessfullySensed());
		
		//CanSeePlayer�� true�� �÷��̾ ���� �Ѱ� �̰� false�� ���� ���� ���� ������ �Ǵ���. 
		if (GetBlackboardComponent()->GetValueAsBool(CanSeePlayer))
		{
			// Player �� �÷��̾� ĳ���� ���� �� �̵� �ӵ� ����
			GetBlackboardComponent()->SetValueAsObject(Player, PlayerPawn);
			AEnemy* Enemy = Cast<AEnemy>(GetPawn());
			Enemy->GetCharacterMovement()->MaxWalkSpeed = 550.f;

			AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			SetFocus(Main);
		}
		else
		{
			// �÷��̾ ���� ���� ������ �����ų�, ���� ���� ���� ���̹Ƿ� Player�� nullptr���� �� �̵��ӵ� ����
			GetBlackboardComponent()->SetValueAsObject(Player, nullptr);
			GetBlackboardComponent()->SetValueAsBool(IsInAttackRange, false);
			AEnemy* Enemy = Cast<AEnemy>(GetPawn());
			Enemy->GetCharacterMovement()->MaxWalkSpeed = 300.f;

			AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}

//PerceptionComponent �ʱ�ȭ ��� �Լ�
void AEnemyController::SetPerceptionSystem()
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
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::OnUpdated);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetDetected);
}