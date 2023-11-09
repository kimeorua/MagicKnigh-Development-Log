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
	APawn* Pawn = TryGetPawnOwner(); // Pawn 변수에 플레이어 캐릭터 저장

	if (::IsValid(Pawn)) // 유효 하면
	{
		Player = Cast<APlayerCharacter>(Pawn); // Main 변수에 저장
		Speed = Player->GetVelocity().Size(); // 캐릭터의 속도를 speed에 저장
		Direction = UKismetAnimationLibrary::CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
		EquipedWeapon = CheackWeaponTag();
		IsBlock = CheackUseBlock();
		bLockOn = Player->GetUseLockOn();
		bHealing = CheackUseHeal();
	}
}

EEquipedWeapon UPlayerAnimInstance::CheackWeaponTag()
{
	if (Player != nullptr) // 플레이어가 유효 하면
	{
		if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Weapon.Sword"))) > 0) //검 장착시
		{
			return  EEquipedWeapon::Sword;
		}
		else if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Weapon.Axe"))) > 0) //도끼 장착시
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
	if (Player != nullptr) //  플레이어가 유효 하면
	{
		if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) > 0) //방어 사용 중이면
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
	if (Player != nullptr) //  플레이어가 유효 하면
	{
		if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseHealing"))) > 0) //회복 스킬 사용 중이면
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
