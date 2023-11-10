// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicKnightEnums.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Q_Skill UMETA(DisplayName = "Q_Skill"), //중간 데미지
	E_Skill UMETA(DisplayName = "E_Skill"), //낮은 데미지
	Special_Skill UMETA(DisplayName = "Special_Skil"), //높은 데미지

	Max UMETA(DisplayName = "Max")
};

UCLASS()
class PF_MAGICKNIGHT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UParticleSystem* GetSoummonsParticle() const { return SoummonParitcle; }
	FORCEINLINE USoundBase* GetSoummonSound() const { return SoummonSound; }
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }

	//오버랩 확인
	UFUNCTION(BlueprintCallable)
	FHitResult CheakCollision(EAttackCollisionType Type, float Range, EAttackDirectionType DriectionType, EDamageEffectType DamgeType);

	//적 배열 초기화
	UFUNCTION(BlueprintCallable)
	void HitArrReset();

	/// <summary>
	/// 공격 몽타주 반환
	/// </summary>
	/// <param name="index">공격 번호</param>
	/// <returns>공격 애님 몽타주</returns>
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetCurrentAttackMontage(int index) const;

	/// <summary>
	/// 스킬 몽타주 반환
	/// </summary>
	/// <param name="Type">스킬 타입</param>
	/// <returns>스킬 애님 몽타주</returns>
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetCurrentSkillMontage(ESkillType Type) const;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoummonParticle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* SoummonParitcle = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoummonParticle", meta = (AllowPrivateAccess = "true"))
	USoundBase* SoummonSound = nullptr;

	// 공격 반지름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRadius = 0.f;

	//Trace Start Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionStartSocket = "";

	//Trace End Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionEndSocket = "";

	//Trace AOE Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionAOESocket = "";

	//공격에 맞은 적 배열
	TArray<class AEnemyCharacter*>HitEnemys;

	class UCombetComponent* CombetComp;

	/// <summary>
	/// 콤보 공격 몽타주
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackMontages;

	/// <summary>
	/// 스킬 몽타주
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TMap<ESkillType, UAnimMontage*> SkillMontages;
};
