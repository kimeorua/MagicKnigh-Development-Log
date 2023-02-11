// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
class AShield;
class AWeapon;

UCLASS()
class MYGAME_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AMainCharacter();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float ForwardWalkSpeed = 350.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float ForwardRunSpeed = 650.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float BackwardSpeed = 100.f;

	float CurrentSpeed;
	bool bUseDash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	int32 MoveNum = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	int32 WeaponNum = 0;

	class UMainAnimInstance* MainAnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AShield> ShieldClass;
	UPROPERTY()
	AShield* Shield = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> SwordClass;
	UPROPERTY()
	AWeapon* Sword = nullptr;
	UPROPERTY()
	AWeapon* CurrentWeapon = nullptr;

	bool IsCombo;

	const int MaxCombo = 4;
	int CurrentCombo;

	//-----------------------------------------Function-----------------------------------------//
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	void Dash();
	void DashEnd();
	void Dodge();

	void BlockStart();
	void BlockEnd();

	void SelectSword();
	void WeaponEquip();

	void LMBDawn();
	void Attack();

	void QSkillActivated();
	void ESkillActivated();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bUseBlock = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combet)
	int32 UseWeaponNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	MoveState State;

	void DodgeEnd();
	void AttackEnd();
	void CheackCombo();
	AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
};
