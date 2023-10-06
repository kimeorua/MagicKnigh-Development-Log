// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicKnightEnums.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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

	//오버랩 확인
	UFUNCTION(BlueprintCallable)
	FHitResult CheakCollision(EAttackCollisionType Type, float Range, EDamageEffectType DamgeType);

	//적 배열 초기화
	UFUNCTION(BlueprintCallable)
	void HitArrReset();

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
};
