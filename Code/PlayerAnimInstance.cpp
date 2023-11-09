// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "KismetAnimationLibrary.h"

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
		Direction = UKismetAnimationLibrary::CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
		EquipedWeapon = CheackWeaponTag();
		IsBlock = CheackUseBlock();
		bLockOn = Player->GetUseLockOn();
		bHealing = CheackUseHeal();
	}
}

EEquipedWeapon UPlayerAnimInstance::CheackWeaponTag()
{
	if (Player != nullptr) // �÷��̾ ��ȿ �ϸ�
	{
		if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Weapon.Sword"))) > 0) //�� ������
		{
			return  EEquipedWeapon::Sword;
		}
		else if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Weapon.Axe"))) > 0) //���� ������
		{
			return  EEquipedWeapon::Axe;
		}
		else
		{
			return  EEquipedWeapon::None;
		}
	}
	else
	{
		return  EEquipedWeapon::None;
	}
}

bool UPlayerAnimInstance::CheackUseBlock()
{
	if (Player != nullptr) //  �÷��̾ ��ȿ �ϸ�
	{
		if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) > 0) //��� ��� ���̸�
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UPlayerAnimInstance::CheackUseHeal()
{
	if (Player != nullptr) //  �÷��̾ ��ȿ �ϸ�
	{
		if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseHealing"))) > 0) //ȸ�� ��ų ��� ���̸�
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
