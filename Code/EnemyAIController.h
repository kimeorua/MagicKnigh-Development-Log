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
	static const FName EnemyPatternNum;
	//-------------------------------------------블랙보드 키에 접근시 사용할 이름들-------------------------------------------//

	//-------------------------------------------------AI Perception 변수-------------------------------------------------//
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
