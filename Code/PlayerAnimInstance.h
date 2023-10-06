// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicKnightEnums.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	//ĳ������ �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Speed; 

	//ĳ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Direction; 

	// �÷��̾� ĳ���� 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* Player = nullptr; 

	//ĳ���Ͱ� ���� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	EEquipedWeapon EquipedWeapon;

	//��� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool IsBlock;

	//LockOn ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	bool bLockOn;

	//���� ��ų ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool bHealing;

public:
	UPlayerAnimInstance(); // ������
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�ִϸ��̼� ������Ʈ �Լ�

	//�÷��̾ ������ ���⸦ GameplayTag�� ���� Ȯ��
	EEquipedWeapon CheackWeaponTag(); 
	//GameplayTag�� ���� ��� ��� ���Ҹ� Ȯ��
	bool CheackUseBlock();
	//GameplayTag�� ���� ȸ����ų ��� ���θ� Ȯ��
	bool CheackUseHeal();
};
