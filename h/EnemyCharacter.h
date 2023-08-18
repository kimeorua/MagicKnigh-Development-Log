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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	FVector StartPoint; //처음 위치

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	int PointNum; //순찰할 장소 숫자

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName AnimSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool bIsFristAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* HitCollision;

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

	UFUNCTION(BlueprintCallable)
	void PlayerHitReset();

	UFUNCTION(BlueprintCallable)
	FHitResult CheakCollision(EAttackCollisionType Type, float Range);

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetPatrolPoints() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetStartPoint() const { return StartPoint; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetPatrolPointNum() const { return PointNum; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPatrolPointNum(int Num) { PointNum = Num; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetAnimSection() const { return AnimSection; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetAnimSection(FName SectionName) { AnimSection = SectionName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsFristAttack() const { return bIsFristAttack; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsFristAttack(bool Cheack) { bIsFristAttack = Cheack; }
};
