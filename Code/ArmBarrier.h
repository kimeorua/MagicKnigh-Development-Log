// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmBarrier.generated.h"

UCLASS()
class PF_MAGICKNIGHT_API AArmBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmBarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }

	UFUNCTION(BlueprintCallable)
	void BarrierOn(); //방패 크기 확 대
	void BarrierOff(); //방패 크기 원복

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	FName EquipSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundBase* BarrierOnSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundBase* BarrierOffSound = nullptr;

	bool IsBarrierOnEffect = false; //방패 크기 증가 여부
};
