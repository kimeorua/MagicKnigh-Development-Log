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

	/// <summary>
	/// ������ ��Ĺ �̸� ��ȯ
	/// </summary>
	/// <returns>EquipSocketName</returns>
	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }

	/// <summary>
	/// ���� ũ�� ����
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void BarrierOn(); 

	/// <summary>
	/// ���� ũ�� ����
	/// </summary>
	void BarrierOff();

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

	bool IsBarrierOnEffect = false; //���� ũ�� ���� ����
};
