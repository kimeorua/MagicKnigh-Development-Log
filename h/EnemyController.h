// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

/**
 * 
 */
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
    static const FName Player;
    static const FName IsInAttackRange;

    //AI Perception 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AISightRadius = 800;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AILoseSightRadius = 100.f;

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
    
};
