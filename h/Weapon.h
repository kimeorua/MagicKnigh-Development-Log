// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoummonParticle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* SoummonParitcle = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoummonParticle", meta = (AllowPrivateAccess = "true"))
	USoundBase* SoummonSound = nullptr;

};
