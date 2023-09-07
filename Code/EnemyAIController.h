// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree; //�����̺�� Ʈ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent; //�����̺�� Ʈ�� ������Ʈ

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent; //������ ������Ʈ

	class UAISenseConfig_Sight* SightConfig; //�ð� ���� ������Ʈ

	class AEnemyCharacter* Enemy;

protected:
	void BeginPlay()override;
	virtual void OnPossess(APawn* InPawn) override;     //�� ���ǽ� ȣ�� �Ǵ� �Լ�


public:
	//-------------------------------------------������ Ű�� ���ٽ� ����� �̸���-------------------------------------------//
	static const FName CanSeePlayer;
	static const FName Player;
	static const FName SelfActor;
	static const FName Distance;
	static const FName TargetLocation;
	static const FName PatrolNum;
	static const FName EnemyPatternNum;
	//-------------------------------------------������ Ű�� ���ٽ� ����� �̸���-------------------------------------------//

	//-------------------------------------------------AI Perception ����-------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightRadius = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILoseSightRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AIFieldOfView = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightAge = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILastSeenLocation = 100.f;

	//-------------------------------------------------�Լ�-------------------------------------------------//
	AEnemyAIController();
	
	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& updated_actors);
	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	UFUNCTION()
	void SetPerceptionSystem();

	//������ ������Ʈ ��ȯ
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return BehaviorTreeComponent; }
};
