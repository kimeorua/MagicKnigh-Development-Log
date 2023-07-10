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
