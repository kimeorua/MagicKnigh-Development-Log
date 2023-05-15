// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameData.h"
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> ComboAttackMontage; // �޺� ���� �迭

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleePatternMontage; //���� ���� �ִϸ��̼�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleePattern2Montage; //���� ���� �ִϸ��̼�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CurrentComboAttackMontage; //�޺����� �ִϸ��̼�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage; //�޺����� �ִϸ��̼�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* StrunMontage; //�޺����� �ִϸ��̼�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DieMontage; //�޺����� �ִϸ��̼�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	int ComboNum;

	class AEnemyController* Controller;

	//�ִ� �ν��Ͻ�
	class UEnemyAnimInstance* EnemyAnimInstance;
	//------------------------Function-----------------------//
    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void ComboUp(); //�޺� ���� �Լ�

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE void ComboReset() { ComboNum = 0; } //�޺� �ʱ�ȭ �Լ�

	UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE UAnimMontage* GetHitMontage() { return HitMontage; } 

	UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE UAnimMontage* GetStrunMontage() { return StrunMontage; }

	UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE UAnimMontage* GetDieMontage() { return DieMontage; }

	UFUNCTION(BlueprintCallable,  meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GetCurrentCombo(); //���� �޺��� �´� �ִϸ��̼� ��ȯ

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void RandomPattern();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AEnemy();
};
