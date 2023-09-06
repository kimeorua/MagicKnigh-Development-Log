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
	APawn* Pawn = TryGetPawnOwner(); // Pawn ������ �÷��̾� ĳ���� ����

	if (::IsValid(Pawn)) // ��ȿ �ϸ�
	{
		Enemy = Cast<AEnemyCharacter>(Pawn); // Main ������ ����
		Speed = Enemy->GetVelocity().Size(); // ĳ������ �ӵ��� speed�� ����
		Direction = UKismetAnimationLibrary::CalculateDirection(Enemy->GetVelocity(), Enemy->GetActorRotation());
	}
}
