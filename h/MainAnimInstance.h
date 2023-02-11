// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Speed; //ĳ������ �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Direction; //ĳ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool bIsBlock = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* Main = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	int32 UseWeaponNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ESkillMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* QSkillMontage;

public:
	UMainAnimInstance(); // ������
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�ִϸ��̼� ������Ʈ �Լ�

	void PlayDodge(int32 num);
	void PlayEquip();
	void PlayAttack(int CurrentCombo);

	void PlaySkill(char type);

	UFUNCTION()
	void AnimNotify_DodgeEnd();

	UFUNCTION()
	void AnimNotify_EquipEnd();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_CheackCombo();
};
