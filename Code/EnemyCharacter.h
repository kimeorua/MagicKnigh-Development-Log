// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate); //��������Ʈ ����

UCLASS()
class PF_MAGICKNIGHT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class AEnemyAIController* AIController = nullptr; //AI��Ʈ�ѷ�

	//�ȱ� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 380.0f;

	//�޸��� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 550.0f;

	// ������ ��Ʈ�� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	TArray <class ATargetPoint*> PatrolPoints; 

	//�ǰ� ���� �ݸ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* HitCollision;

	//Trace Start Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionStartSocket_L = "";

	//Trace End Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionEndSocket_L = "";

	//Trace Start Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionStartSocket_R = "";

	//Trace End Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionEndSocket_R = "";

	// �÷��̾ ���ݿ� �¾Ҵ°�?
	bool PlayerIsHit = false;

	//�ǰݽ� ��ƼŬ ���� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	FName HitParticleSocket = "";

	//�ǰ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	USoundBase* HitSound;

	//�ǰ� ��ƼŬ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* UI;

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

public:
	//���������� -> ���� �ִϸ��̼� �Ϸ��, AI�� ����Task ����
	FOnAttackEndDelegate OnAttackEnd;

	//�ִϸ��̼� ȣ���� ���� ����
	UPROPERTY()
	class UEnemyAnimInstance* EnemyAnim; 

	AEnemyCharacter();

	//�÷��̾� ã�� -> �̵��ӵ� ����
	void FindPlayer();

	//�÷��̾� ��ħ -> �̵��ӵ� ����
	void LosePlayer();

	//�÷��̾�� ������ ����
	void TakeDamgeFormPlayer(EDamageEffectType DamageType);

	//�÷��̾ �� ĳ������ ������ ƨ�ܳ� -> ü�� ���
	void TakeParrying();

	//��������Ʈ �۵�
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//���ݿ� ���� �÷��̾� �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
	void PlayerHitReset();

	//���� ����
	UFUNCTION(BlueprintCallable)
	FHitResult CheakCollision(EAttackCollisionType Type, float Range, EDamageEffectType DamageType, EAttackDirectionType DriectionType);

	//��Ʈ�� ����Ʈ �迭
	FORCEINLINE TArray <class ATargetPoint*> GetPatrolPoint() const { return PatrolPoints; }

	//��� �Լ� override
	void Die() override;
};
