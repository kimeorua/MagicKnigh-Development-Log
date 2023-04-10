// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Speed; //캐릭터의 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float Direction; //캐릭터의 방향

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	class AEnemy* OwnerCharacter = nullptr; // 메인캐릭터 

public:
	UEnemyAnimInstance(); // 생성자
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //애니메이션 업데이트 함수
};
