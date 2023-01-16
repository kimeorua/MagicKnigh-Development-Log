// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
class AShield;

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

	class UMainAnimInstance* MainAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	MoveState State;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AShield> ShieldClass;

	UPROPERTY()
	AShield* Shield;

	//-----------------------------------------Function-----------------------------------------//
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	void Dash();
	void DashEnd();
	void Dodge();


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

	void DodgeEnd();
};
