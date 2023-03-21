// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class MYGAME_API AWeapon : public AActor
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

	FORCEINLINE UAnimMontage* GetAttackMontage() const { return ComboAttackMontage; }
	FORCEINLINE int GetAttackMaxCombo() const { return MaxCombo; }

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	//������ ���� -> ������ �󿡼� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	FName EquipSocket;

	//���� ��ȣ -> �ִϸ��̼� �����, ������ �󿡼� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	int32 EquipNum;

	// ���� �ִϸ��̼� -> ������ �󿡼� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMontage = nullptr;

	// ���� �ִϸ��̼� -> ������ �󿡼� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ComboAttackMontage = nullptr;

	//��ų �迭 ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*>ESkils;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*>QSkils;

	//������ ���Ǵ� ��ų
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* UseESkils;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* UseQSkils;

	//Ư�� ��ų
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EPSkillMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	int MaxCombo;
};
