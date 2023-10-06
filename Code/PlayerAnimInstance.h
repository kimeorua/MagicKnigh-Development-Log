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
	//캐릭터의 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Speed; 

	//캐릭터의 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Direction; 

	// 플레이어 캐릭터 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* Player = nullptr; 

	//캐릭터가 현재 장착한 무기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	EEquipedWeapon EquipedWeapon;

	//방어 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool IsBlock;

	//LockOn 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	bool bLockOn;

	//힐링 스킬 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool bHealing;

public:
	UPlayerAnimInstance(); // 생성자
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //애니메이션 업데이트 함수

	//플레이어가 장착한 무기를 GameplayTag를 통해 확인
	EEquipedWeapon CheackWeaponTag(); 
	//GameplayTag를 통해 방어 사용 여불를 확인
	bool CheackUseBlock();
	//GameplayTag를 통해 회복스킬 사용 여부를 확인
	bool CheackUseHeal();
};
