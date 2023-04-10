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
	APawn* Pawn = TryGetPawnOwner(); // Pawn ������ �÷��̾� ĳ���� ����

	if (::IsValid(Pawn)) // ��ȿ �ϸ�
	{
		OwnerCharacter = Cast<AEnemy>(Pawn); // Main ������ ����
		Speed = OwnerCharacter->GetVelocity().Size(); // ĳ������ �ӵ��� ���� speed ����ȭ
		Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation()); //���� ������ ����Ͽ� ����
	}
}
