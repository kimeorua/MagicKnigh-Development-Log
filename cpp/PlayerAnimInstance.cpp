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
	}
}

EEquipedWeapon UPlayerAnimInstance::CheackWeaponTag()
{
	if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Weapon.Sword"))) > 0)
	{
		return  EEquipedWeapon::Sword;
	}
	else
	{
		return  EEquipedWeapon::None;
	}
}
