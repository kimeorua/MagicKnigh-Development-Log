// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Speed; //ĳ������ �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Direction; //ĳ������ ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* Player = nullptr; // �÷��̾� ĳ���� 

public:
	UPlayerAnimInstance(); // ������
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�ִϸ��̼� ������Ʈ �Լ�
};
