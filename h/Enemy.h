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
	TArray<UAnimMontage*> ComboAttackMontage; // 콤보 공격 배열

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleePatternMontage; //근접 패턴 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleePattern2Montage; //근접 패턴 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CurrentComboAttackMontage; //콤보공격 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage; //콤보공격 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* StrunMontage; //콤보공격 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DieMontage; //콤보공격 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	int ComboNum;

	class AEnemyController* Controller;

	//애님 인스턴스
	class UEnemyAnimInstance* EnemyAnimInstance;
	//------------------------Function-----------------------//
    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void ComboUp(); //콤보 증가 함수

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE void ComboReset() { ComboNum = 0; } //콤보 초기화 함수

	UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE UAnimMontage* GetHitMontage() { return HitMontage; } 

	UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE UAnimMontage* GetStrunMontage() { return StrunMontage; }

	UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE UAnimMontage* GetDieMontage() { return DieMontage; }

	UFUNCTION(BlueprintCallable,  meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GetCurrentCombo(); //현제 콤보에 맞는 애니메이션 반환

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
