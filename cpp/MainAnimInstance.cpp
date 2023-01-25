// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "MainCharacter.h"

UMainAnimInstance::UMainAnimInstance()
{
	Speed = 0.f;
	Direction =  0.f;
	DodgeMontage = nullptr;
	EquipMontage = nullptr;
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Pawn = TryGetPawnOwner(); // Pawn ������ �÷��̾� ĳ���� ����

	if (::IsValid(Pawn)) // ��ȿ �ϸ�
	{
		AMainCharacter* Main = Cast<AMainCharacter>(Pawn); // Main ������ ����
		Speed = Main->GetVelocity().Size(); // ĳ������ �ӵ��� ���� speed ����ȭ
		Direction = CalculateDirection(Main->GetVelocity(), Main->GetActorRotation());
		bIsBlock = Main->bUseBlock;
		UseWeapon = Main->UseWeaponNum;
	}
}

void UMainAnimInstance::PlayDodge(int32 num)
{
	if (!Montage_IsPlaying(DodgeMontage))
	{
		if (num == 1)
		{
			Montage_JumpToSection("Forward", DodgeMontage);
			Montage_Play(DodgeMontage);
		}
		else if (num == 2)
		{
			Montage_Play(DodgeMontage);
			Montage_JumpToSection("Backward", DodgeMontage);
		}
		else if (num == 3)
		{
			Montage_Play(DodgeMontage);
			Montage_JumpToSection("Right", DodgeMontage);
		}
		else if (num == 4)
		{
			Montage_Play(DodgeMontage);
			Montage_JumpToSection("Left", DodgeMontage);
		}
		else
		{
			return;
		}
	}
}

void UMainAnimInstance::PlayEquip()
{
	if (!Montage_IsPlaying(EquipMontage))
	{
		Montage_Play(EquipMontage);
	}
}

void UMainAnimInstance::AnimNotify_DodgeEnd()
{
	APawn* Pawn = TryGetPawnOwner(); // Pawn ������ �÷��̾� ĳ���� ����

	if (::IsValid(Pawn)) // ��ȿ �ϸ�
	{
		AMainCharacter* Main = Cast<AMainCharacter>(Pawn); // Main ������ ����
		Main->DodgeEnd();
	}
}
