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
	float Speed; //캐릭터의 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Direction; //캐릭터의 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool bIsBlock = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	int32 UseWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMontage;

public:
	UMainAnimInstance(); // 생성자
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //애니메이션 업데이트 함수

	void PlayDodge(int32 num);

	void PlayEquip();

	UFUNCTION()
	void AnimNotify_DodgeEnd();
};
