// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	Speed = 0.f;
	Direction = 0.f;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Pawn = TryGetPawnOwner(); // Pawn ������ �÷��̾� ĳ���� ����

	if (::IsValid(Pawn)) // ��ȿ �ϸ�
	{
		Player = Cast<APlayerCharacter>(Pawn); // Main ������ ����
		Speed = Player->GetVelocity().Size(); // ĳ������ �ӵ��� speed�� ����
		Direction = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation()); //���� ������ ����Ͽ� ����
	}
}