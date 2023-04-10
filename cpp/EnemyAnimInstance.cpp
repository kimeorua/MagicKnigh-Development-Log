// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	Speed = 0.f;
	Direction = 0.f;
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Pawn = TryGetPawnOwner(); // Pawn 변수에 플레이어 캐릭터 저장

	if (::IsValid(Pawn)) // 유효 하면
	{
		OwnerCharacter = Cast<AEnemy>(Pawn); // Main 변수에 저장
		Speed = OwnerCharacter->GetVelocity().Size(); // 캐릭터의 속도와 변수 speed 동기화
		Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation()); //현제 방향을 계산하여 저장
	}
}
