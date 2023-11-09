// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate); //델리게이트 선언

UCLASS()
class PF_MAGICKNIGHT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class AEnemyAIController* AIController = nullptr; //AI컨트롤러

	//걷기 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 380.0f;

	//달리기 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 550.0f;

	// 순찰할 패트롤 포인트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	TArray <class ATargetPoint*> PatrolPoints; 

	//피격 판정 콜리전
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

	// 플레이어가 공격에 맞았는가?
	bool PlayerIsHit = false;

	//피격시 파티클 스폰 위치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	FName HitParticleSocket = "";

	//피격 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	USoundBase* HitSound;

	//피격 파티클
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
	//델리게이터 -> 공격 애니메이션 완료시, AI의 공격Task 종료
	FOnAttackEndDelegate OnAttackEnd;

	//애니메이션 호출을 위한 변수
	UPROPERTY()
	class UEnemyAnimInstance* EnemyAnim; 

	/// <summary>
	/// 생성자
	/// </summary>
	AEnemyCharacter();

	
	/// <summary>
	/// 플레이어 찾음 -> 이동속도 증가
	/// </summary>
	void FindPlayer();

	/// <summary>
	/// 플레이어 놓침 -> 이동속도 감소
	/// </summary>
	void LosePlayer();

	/// <summary>
	/// 플레이어에게 데미지 받음
	/// </summary>
	/// <param name="DamageType">데미지 타입(약, 강, 특수, 보통)</param>
	void TakeDamgeFormPlayer(EDamageEffectType DamageType);

	/// <summary>
	/// 플레이어가 적 캐릭터의 공격을 튕겨냄 -> 체간 상승
	/// </summary>
	void TakeParrying();

	/// <summary>
	/// 델리게이트 작동
	/// </summary>
	/// <param name="Montage">애님 몽타주</param>
	/// <param name="bInterrupted">중단 여부</param>
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/// <summary>
	/// 공격에 맞은 플레이어 초기화
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void PlayerHitReset();

	/// <summary>
	/// 공격 판정
	/// </summary>
	/// <param name="Type">공격 타입</param>
	/// <param name="Range">공격 범위</param>
	/// <param name="DamageType">데미지 타입</param>
	/// <param name="DriectionType">공격 방향</param>
	/// <returns>FHitResult 구조체</returns>
	UFUNCTION(BlueprintCallable)
	FHitResult CheakCollision(EAttackCollisionType Type, float Range, EDamageEffectType DamageType, EAttackDirectionType DriectionType);

	/// <summary>
	/// 패트롤 포인트 배열 반환
	/// </summary>
	/// <returns>PatrolPoints</returns>
	FORCEINLINE TArray <class ATargetPoint*> GetPatrolPoint() const { return PatrolPoints; }

	/// <summary>
	/// 사망 함수 override
	/// </summary>
	void Die() override;

	/// <summary>
	/// 타겟팅 시 바닥에 원형 장판 표시 -> 가시성 개선용
	/// </summary>
	void OnTargeting();

	FORCEINLINE FName GetID() const { return ID; }
};
