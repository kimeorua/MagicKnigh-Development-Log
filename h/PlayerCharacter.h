// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// ī�޶� 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//MappingContext 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	// W,A,S,D Move Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	//���� ���� Look Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	//�޸��� Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;

	//ȸ�� Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DodgeAction;

	// ���� ���� Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<class UInputAction*>EquipActions;

	// �Ϲݰ��� Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ComboAction;

	// ��ų ����(Q) Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QSkillAction;

	// ��ų����(E) Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ESkillAction;

	// Ư�� ��ų(EF)���� Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EFSkillAction;

	// ��� �Է� Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlockAction;

	//���� ȸ�� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float LookRate = 0.35f;

	//�ȱ� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 380.0f;
	
	//�޸��� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 650.0f;

	//----------------------------------------------------------------------����-----------------------------------------------------------------------\\

	//�� �������Ʈ ���� -> �����Ϳ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeapon> SwordClass;

	//class���� ����� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class AWeapon* Sword;

	// ������ ��� �迭 0  = ��, 1 = ����  ....(���Ⱑ �þ�� ���� �߰� �� ��)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TArray<FName>EquipSockets;

	//���� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class AWeapon* CurrentWeapon;

	//����  �������Ʈ ���� -> �����Ϳ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AArmBarrier> ArmBarrierClass;

	//class���� ����� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class AArmBarrier* ArmBarrier;

	//----------------------------------------------------------------------����-----------------------------------------------------------------------\\

public:
	// ������
	APlayerCharacter();

	void PlayerSetup();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	//------------------------ ī�޶� �� SpringArm ��ȯ ------------------------//
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	//--------------------------------------------------------------------------------//

	//�±� ���� �� �ش� �±׸� ���� GameplayAbility �۵�
	void MakeTagAndActive(FString TagName);

	//W, A, S, D ������ 
	void Move(const FInputActionValue& Value);

	// ������ ���� -> Tag ����
	void MoveEnd();

	// ���� ����
	void Look(const FInputActionValue& Value);

	// �޸��� ��� �� ����
 	void Dash();
	void DashEnd();

	//ȸ�� ���
	void Dodge();

	// �Ϲ� ���� ���(�޺�)
	void ComboAttack();

	//Q Skill ���
	void UseQSkill();

	// E Skill ���
	void UseESkill();

	// EF Skill ���
	void UseEFSkill();

	//��� ���� �� ����
	void BlockStart();
	void BlockEnd();

	//���� ��ȯ -> ��������(�ٸ� ���� ���� �� �϶�) -> ����
	void SwordSummons();
	void WeaponUnequip();
	// ���� ������ �������Ʈ���� ȣ�� -> ���� �̵� �Ϸ��� ���� ��
	UFUNCTION(BlueprintCallable)
	void WeaponEquip(FName EquipSocketName, AWeapon* Weapon);
};
