// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyCharacter.h"
#include "KismetAnimationLibrary.h"


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
		Enemy = Cast<AEnemyCharacter>(Pawn); // Main 변수에 저장
		Speed = Enemy->GetVelocity().Size(); // 캐릭터의 속도를 speed에 저장
		Direction = UKismetAnimationLibrary::CalculateDirection(Enemy->GetVelocity(), Enemy->GetActorRotation());
	}
}
