// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
class AShield; //���� class
class AWeapon; //���� class

UCLASS()
class MYGAME_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AMainCharacter();

	bool GetUseBlock() { return bUseBloack;  } //��� ������ Ȯ�� �ϴ� ���� -> �ִϸ��̼� ��� ���� Get�� ����

private:

	//ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	//-----------------------------------------------------------------------------------------------------//
	
	//�̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float ForwardWalkSpeed = 350.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float ForwardRunSpeed = 650.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float BackwardSpeed = 100.f;

	float CurrentSpeed; //���� �̵� �ӵ�
	//-----------------------------------------------------------------------------------------------------//

	bool bUseDash; //�޸��� ��� ����

	//��� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bUseBloack;

	//�����Ƽ ��� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bCanUseAbility;

	//���� �����Ƽ ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bUseAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bIsHit;

	//�̵� ����� ����(ȸ�� �ִϸ��̼� ���� ��)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	int32 MoveNum;

	//���� ���� ������ Ȯ��(C++ ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool IsAttack;

	//���� ȸ�� ������ �Ǵ�(DodgeControll �ִԳ�Ƽ���� ������Ʈ���� ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool IsDodge;

	//�ִ��ν��Ͻ� ����
	class UMainAnimInstance* MainAnimInstance;

	//����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AShield> ShieldClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	AShield* Shield = nullptr;
	//-----------------------------------------------------------------------------------------------------//

	//��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> SwordClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	AWeapon* Sword = nullptr;

	//��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> AxeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	AWeapon* Axe = nullptr;
	//-----------------------------------------------------------------------------------------------------//

	//���� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	AWeapon* CurrentWeapon = nullptr;
	
	//���� ��� �ϴ� ������ ���� ��ȣ(�ִϸ��̼� ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	int32 CurrentWeaponNum= 0;

	//-----------------------------------------Function-----------------------------------------//

	//�⺻ ������
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	//-----------------------------------------------------------------------------------------------------//

	//�޸���
	void Dash();
	void DashEnd();
	//-----------------------------------------------------------------------------------------------------//

	//ȸ��
	void Dodge();

	// ���� ���� �Լ�--�������Ʈ���� ȣ�� ���� �ϰ� ����
	UFUNCTION(BlueprintCallable)
	bool Equip(AWeapon* UseWeapon, FName EquipSocket, int32 EquipNum);
		
	//ī�޶� ���� ������Ʈ ��ȯ�� �Լ�
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void PlayerOnHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���� ���� ��ȣ ��ȯ�� �Լ�
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentWeaponNum() const { return CurrentWeaponNum; }

	//���� ��ȯ�� �Լ�
	FORCEINLINE class AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintCallable)
	bool CheackCanUseAbility() const;

	UFUNCTION(BlueprintCallable)
	AWeapon* CheackCanUseSkillAbility() const;

	void AttackEnd(); // ���� ���� �Լ�
	void CheackCombo(); // �޺� üũ�� �Լ�
};
