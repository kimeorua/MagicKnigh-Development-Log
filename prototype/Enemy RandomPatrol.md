### Enemy RandomPatrol 구현(03/29)
+ #### AI제작을 위해, EnemyController와 비헤이비어트리, 블랙보드를 이용하여, 맵상의 랜덤한 위치로 이동하는 RandomPatrol을 구현함
+ #### EnemyController에 AIPerception컴포넌트를 통해, 플레이어를 인지 할수 있게 구현.

EnemyController.h

```cpp
UCLASS()
class MYGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()
private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UBehaviorTree* BehaviorTree; //비헤이비어 트리 에셋

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UBehaviorTreeComponent* BehaviorTreeComponent; //비헤이비어트리 컴포넌트

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UBlackboardComponent* BlackboardComponent;    //블랙보드 컴포넌트

    class UAISenseConfig_Sight* SightConfig;

public:
    //블랙보드 키에 접근시 사용할 이름들
    static const FName TargetLocation;
    static const FName CanSeePlayer;

    //AI Perception 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AISightRadius = 500.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AILoseSightRadius = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AIFieldOfView = 90.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AISightAge = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AILastSeenLocation = 100.f;

    //------------------------Function-----------------------//
   UFUNCTION()
    void OnUpdated(TArray<AActor*> const& updated_actors);
    UFUNCTION()
    void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
    UFUNCTION()
    void SetPerceptionSystem();

    AEnemyController(); //생성자

    //블랙보드 컴포넌트 반환
    FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }

protected:
    void BeginPlay()override;
    virtual void OnPossess(APawn* InPawn) override;     //폰 빙의시 호출 되는 함수
```

EnemyController.cpp

```cpp

//접근할 이름에 블랙보드 키값 할당
const FName AEnemyController::TargetLocation(TEXT("TargetLocation"));
const FName AEnemyController::CanSeePlayer(TEXT("CanSeePlayer"));

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
	if (auto const player = Cast<AMainCharacter>(actor))
	{
		//성공적으로 감지하면 블랙보드에 true값을 넣어준다.
		GetBlackboardComponent()->SetValueAsBool(CanSeePlayer, Stimulus.WasSuccessfullySensed());
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
```

+ #### 블랙보드와 비헤이비어를 통하여 실질적인 AI 를 관리함.

비헤이비어 트리

![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%ED%8A%B8%EB%A6%AC.PNG?raw=true)

+ #### RandomPatrol: 맵상의 랜덤 좌표를 지정하여, 블랙보드에 기입.

```cpp

UFindPatrolPosTask::UFindPatrolPosTask(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("FindPatrolPosTask");
}

EBTNodeResult::Type UFindPatrolPosTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Controller = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	auto Enemy = Controller->GetPawn();

	if (nullptr == Enemy) 
	{
		return EBTNodeResult::Failed;
	}
	//현재 에디터에 설정된 navi mesh로 초기화
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (nullptr == NavSystem) 
	{
		//navi mesh가 없다면 실패를 반환
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Enemy in Navi"));
		return EBTNodeResult::Failed;
	}

	FVector const Origin = Enemy->GetActorLocation();
	FNavLocation NextPatrol;

	//NextPatrol변수에 임의의 location 데이터를 넣고 다시 TargetLocation키의 value에 값을 넣어준다.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, NextPatrol))
	{
		Controller->GetBlackboardComponent()->SetValueAsVector(AEnemyController::TargetLocation, NextPatrol.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

```
