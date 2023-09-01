// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MagicKnightEnums.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class AEnemyAIController* AIController; //AI컨트롤러

	//걷기 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 380.0f;

	//달리기 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 550.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	TArray <class ATargetPoint*> PatrolPoints; // 순찰할 패트롤 포인트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* HitCollision;

	// 0 -> Hit Effect, 1-> TakeDamage(Attack) 2-> ParryingAble
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TArray< TSubclassOf<class UGameplayEffect>> HitEffects; //사용하는 데미지용 이펙트들(에디터 설정)

	//Trace Start Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionStartSocket = "";

	//Trace End Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionEndSocket = "";

	bool PlayerIsHit = false;

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;

public:
	AEnemyCharacter();
	void FindPlayer();
	void LosePlayer();
	void TakeDamgeFormPlayer();
	void TakeParrying();

	UFUNCTION(BlueprintCallable)
	void PlayerHitReset();

	UFUNCTION(BlueprintCallable)
	FHitResult CheakCollision(EAttackCollisionType Type, float Range);

	FORCEINLINE TArray <class ATargetPoint*> GetPatrolPoint() const { return PatrolPoints; }
};
