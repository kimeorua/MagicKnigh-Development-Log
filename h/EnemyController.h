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
    class UBehaviorTree* BehaviorTree; //�����̺�� Ʈ�� ����

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UBehaviorTreeComponent* BehaviorTreeComponent; //�����̺��Ʈ�� ������Ʈ

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UBlackboardComponent* BlackboardComponent;    //������ ������Ʈ

    class UAISenseConfig_Sight* SightConfig;

public:
    //������ Ű�� ���ٽ� ����� �̸���
    static const FName TargetLocation;
    static const FName CanSeePlayer;
    static const FName Player;
    static const FName IsInAttackRange;

    //AI Perception ����
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

    AEnemyController(); //������

    //������ ������Ʈ ��ȯ
    FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }

protected:
    void BeginPlay()override;
    virtual void OnPossess(APawn* InPawn) override;     //�� ���ǽ� ȣ�� �Ǵ� �Լ�
    
};
