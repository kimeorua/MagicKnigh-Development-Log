// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "MainCharacter.h"
#include "Weapon.h"
#include "RPGHitComponent.h"

UMainAnimInstance::UMainAnimInstance()
{
	//초기화
	Speed = 0.f;
	Direction =  0.f;

	DodgeMontage = nullptr;
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds) //애니메이션 업데이트 시 호출
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Pawn = TryGetPawnOwner(); // Pawn 변수에 플레이어 캐릭터 저장

	if (::IsValid(Pawn)) // 유효 하면
	{
		Main = Cast<AMainCharacter>(Pawn); // Main 변수에 저장
		Speed = Main->GetVelocity().Size(); // 캐릭터의 속도와 변수 speed 동기화
		Direction = CalculateDirection(Main->GetVelocity(), Main->GetActorRotation()); //현제 방향을 계산하여 저장
		bIsBlock = Main->GetUseBlock(); //현제 방어 여부를 저장
		CurrentWeaponNum = Main->GetCurrentWeaponNum(); //현제 무기의 고유 번호 저장
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

void UMainAnimInstance::AnimNotify_AttackEnd()
{
	Main->AttackEnd();
}