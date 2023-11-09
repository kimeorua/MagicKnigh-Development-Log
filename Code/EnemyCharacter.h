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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* UI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* TargetingDecal;

	// Enemy ID 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ID", meta = (AllowPrivateAccess = "true"))
	FName ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnLockWeapon", meta = (AllowPrivateAccess = "true"))
	bool UnLockAble;


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

	/// <summary>
	/// ������
	/// </summary>
	AEnemyCharacter();

	
	/// <summary>
	/// �÷��̾� ã�� -> �̵��ӵ� ����
	/// </summary>
	void FindPlayer();

	/// <summary>
	/// �÷��̾� ��ħ -> �̵��ӵ� ����
	/// </summary>
	void LosePlayer();

	/// <summary>
	/// �÷��̾�� ������ ����
	/// </summary>
	/// <param name="DamageType">������ Ÿ��(��, ��, Ư��, ����)</param>
	void TakeDamgeFormPlayer(EDamageEffectType DamageType);

	/// <summary>
	/// �÷��̾ �� ĳ������ ������ ƨ�ܳ� -> ü�� ���
	/// </summary>
	void TakeParrying();

	/// <summary>
	/// ��������Ʈ �۵�
	/// </summary>
	/// <param name="Montage">�ִ� ��Ÿ��</param>
	/// <param name="bInterrupted">�ߴ� ����</param>
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/// <summary>
	/// ���ݿ� ���� �÷��̾� �ʱ�ȭ
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void PlayerHitReset();

	/// <summary>
	/// ���� ����
	/// </summary>
	/// <param name="Type">���� Ÿ��</param>
	/// <param name="Range">���� ����</param>
	/// <param name="DamageType">������ Ÿ��</param>
	/// <param name="DriectionType">���� ����</param>
	/// <returns>FHitResult ����ü</returns>
	UFUNCTION(BlueprintCallable)
	FHitResult CheakCollision(EAttackCollisionType Type, float Range, EDamageEffectType DamageType, EAttackDirectionType DriectionType);

	/// <summary>
	/// ��Ʈ�� ����Ʈ �迭 ��ȯ
	/// </summary>
	/// <returns>PatrolPoints</returns>
	FORCEINLINE TArray <class ATargetPoint*> GetPatrolPoint() const { return PatrolPoints; }

	/// <summary>
	/// ��� �Լ� override
	/// </summary>
	void Die() override;

	/// <summary>
	/// Ÿ���� �� �ٴڿ� ���� ���� ǥ�� -> ���ü� ������
	/// </summary>
	void OnTargeting();

	FORCEINLINE FName GetID() const { return ID; }
};
