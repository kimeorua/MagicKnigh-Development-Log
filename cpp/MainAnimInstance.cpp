// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "MainCharacter.h"
#include "Weapon.h"

UMainAnimInstance::UMainAnimInstance()
{
	Speed = 0.f;
	Direction =  0.f;
	DodgeMontage = nullptr;
	EquipMontage = nullptr;
	AttackMontage = nullptr;
	ESkillMontage = nullptr;
	QSkillMontage = nullptr;
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Pawn = TryGetPawnOwner(); // Pawn 변수에 플레이어 캐릭터 저장

	if (::IsValid(Pawn)) // 유효 하면
	{
		Main = Cast<AMainCharacter>(Pawn); // Main 변수에 저장
		Speed = Main->GetVelocity().Size(); // 캐릭터의 속도와 변수 speed 동기화
		Direction = CalculateDirection(Main->GetVelocity(), Main->GetActorRotation());
		bIsBlock = Main->bUseBlock;
		UseWeaponNum = Main->UseWeaponNum;
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
	EquipMontage = Main->GetCurrentWeapon()->EquipMontage;
	if (!Montage_IsPlaying(EquipMontage))
	{
		Montage_Play(EquipMontage);
	}
}

void UMainAnimInstance::PlayAttack(int CurrentCombo)
{
	AttackMontage = Main->GetCurrentWeapon()->AttackMontage;
	if (CurrentCombo == 1)
	{
		Montage_Play(AttackMontage);
		Montage_JumpToSection(FName("Attack1"), AttackMontage);
	}
	else if (CurrentCombo == 2)
	{
		///EquipMontage = Main->GetCurrentWeapon()->AttackMontage;
		Montage_Play(AttackMontage);
		Montage_JumpToSection(FName("Attack2"), AttackMontage);
	}
	else if (CurrentCombo == 3)
	{
		//EquipMontage = Main->GetCurrentWeapon()->AttackMontage;
		Montage_Play(AttackMontage, 1.2f);
		Montage_JumpToSection(FName("Attack3"), AttackMontage);
	}
	else if (CurrentCombo == 4)
	{
		//EquipMontage = Main->GetCurrentWeapon()->AttackMontage;
		Montage_Play(AttackMontage, 1.75f);
		Montage_JumpToSection(FName("Attack4"), AttackMontage);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Combo"));
}

void UMainAnimInstance::PlaySkill(char type)
{
	QSkillMontage = Main->GetCurrentWeapon()->QSkillMontage;
	ESkillMontage = Main->GetCurrentWeapon()->ESkillMontage;

	if (type == 'Q')
	{
		if (!Montage_IsPlaying(QSkillMontage))
		{
			Montage_Play(QSkillMontage);
		}
		else { return; }
	}
	else if (type == 'E')
	{
		if (!Montage_IsPlaying(ESkillMontage))
		{
			Montage_Play(ESkillMontage);
		}
		else { return; }
	}
}

void UMainAnimInstance::AnimNotify_DodgeEnd()
{
	Main->DodgeEnd();
}

void UMainAnimInstance::AnimNotify_EquipEnd()
{
	Main->State = MoveState::MS_Move;
}

void UMainAnimInstance::AnimNotify_AttackEnd()
{
	Main->AttackEnd();
}

void UMainAnimInstance::AnimNotify_CheackCombo()
{
	Main->CheackCombo();
}
