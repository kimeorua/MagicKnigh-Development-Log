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
	class AEnemyAIController* AIController; //AI��Ʈ�ѷ�

	//�ȱ� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 380.0f;

	//�޸��� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 550.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	TArray <class ATargetPoint*> PatrolPoints; // ������ ��Ʈ�� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* HitCollision;

	// 0 -> Hit Effect, 1-> TakeDamage(Attack) 2-> ParryingAble
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TArray< TSubclassOf<class UGameplayEffect>> HitEffects; //����ϴ� �������� ����Ʈ��(������ ����)

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
