// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicKnightAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

UMagicKnightAttributeSet::UMagicKnightAttributeSet()
{

}

void UMagicKnightAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth())); //HP의 하한선 및 상한선 설정
	}
	else if (Data.EvaluatedData.Attribute == GetElementalForceAttribute())
	{
		SetElementalForce(FMath::Clamp(GetElementalForce(), 0.f, GetMaxElementalForce())); //EF의 하한선 및 상한선 설정
	}
	else if (Data.EvaluatedData.Attribute == GetPostureAttribute())
	{
		SetPosture(FMath::Clamp(GetPosture(), 0.f, GetMaxPosture())); //체간의 하한선 및 상한선 설정
	}
	else if (Data.EvaluatedData.Attribute == GetHealingCountAttribute())
	{
		SetHealingCount(FMath::Clamp(GetHealingCount(), 0.f, GetMaxHealingCount())); //회복 스킬 횟수의 하한선 및 상한선 설정
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute()) //데미지를 값이 변경됨 -> 데미지를 받음
	{
		//HP 감소
		SetHealth(FMath::Clamp(GetHealth() - Damage.GetCurrentValue(), 0.f, GetMaxHealth()));
		//받은 데미지 * 1.2의 체간 상승
		SetPosture(FMath::Clamp(GetPosture() + Damage.GetCurrentValue() * 1.2f, 0.f, GetMaxPosture()));

		//HP가 0이하 -> 사망 판정
		if (GetHealth() <= 0.f)
		{
			SetHealth(0.f); //HP를 0으로 고정
			ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor()); //부모 클래스 포인터를 이용하여, 적 및 플레이어 캐릭터의 Die(사망 함수)를 호출 함.
			Owner->Die();
		}

		else //HP가 남아 있으면
		{
			if (GetPosture() >= GetMaxPosture()) //체간을 비교하여, 체간 최대치 이상이면 기절 유발
			{
				SetPosture(0.f); //체간 초기화
				ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor());//부모 클래스 포인터를 이용하여, 적 및 플레이어 캐릭터의 Stun(기절 함수)를 호출 함.
				//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Owner->GetName())
				Owner->Stun();
			}
		}

		Damage = 0.f; //데미지 초기화 
	}

	else if (Data.EvaluatedData.Attribute == GetChargeEFAttribute()) //공격 시 EF 충전
	{
		SetElementalForce(FMath::Clamp(GetElementalForce() + ChargeEF.GetCurrentValue(), 0, GetMaxElementalForce())); //충전한 값이 최대치를 넘지 않도록,
		ChargeEF = 0.f; //충전치 초기화
	}

	else if (Data.EvaluatedData.Attribute == GetPostureUpAttribute()) //체간 상승(체력 감소는 없음) -> 가드 성공 시 작동
	{
		SetPosture(FMath::Clamp(GetPosture() + PostureUp.GetCurrentValue(), 0, GetMaxPosture()));

		if (GetPosture() >= GetMaxPosture()) //체간을 비교하여, 체간 최대치 이상이면 기절 유발
		{
			SetPosture(0.f);
			ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor()); //부모 클래스 포인터를 이용하여, 적 및 플레이어 캐릭터의 Stun(기절 함수)를 호출 함.
			//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Owner->GetName())
			Owner->Stun();
		}
		PostureUp = 0.f; //체간 상승치 초기화
	}

	else if (Data.EvaluatedData.Attribute == GetHealing_HPAttribute()) //체력 회복 시
	{
		if (GetHealth() < GetMaxHealth() && GetHealingCount() > 0) //최대 체력일때, 사용횟수가 0보다 작을때는 사용 불가
		{
			SetHealingCount(GetHealingCount() - 1); //횟수 차감
			SetHealth(FMath::Clamp(GetHealth(), GetHealth() + Healing_HP.GetCurrentValue(), GetMaxHealth())); //HP 상승
			//UE_LOG(LogTemp, Warning, TEXT("Heal")); 
		}
		SetHealing_HP(0.f); //회복량 초기화
	}
}

void UMagicKnightAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, Health, COND_None,  REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, ElementalForce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxElementalForce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, Posture, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxPosture, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, ChargeEF, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, HealingCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxHealingCount, COND_None, REPNOTIFY_Always);
}

void UMagicKnightAttributeSet::OnRep_Health(FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Health, OldHealth);
}

void UMagicKnightAttributeSet::OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxHealth, OldMaxHealth);
}

void UMagicKnightAttributeSet::OnRep_Posture(FGameplayAttributeData& OldPosture)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Posture, OldPosture);
}

void UMagicKnightAttributeSet::OnRep_MaxPosture(FGameplayAttributeData& OldMaxPosture)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxPosture, OldMaxPosture);
}

void UMagicKnightAttributeSet::OnRep_ElementalForce(FGameplayAttributeData& OldElementalForce)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, ElementalForce, OldElementalForce);
}

void UMagicKnightAttributeSet::OnRep_MaxElementalForce(FGameplayAttributeData& OldMaxElementalForce)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxElementalForce, OldMaxElementalForce);
}

void UMagicKnightAttributeSet::OnRep_Damage(FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Damage, OldDamage);
}

void UMagicKnightAttributeSet::OnRep_ChargeEF(FGameplayAttributeData& OldChargeEF)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, ChargeEF, OldChargeEF);
}

void UMagicKnightAttributeSet::OnRep_PostureUp(FGameplayAttributeData& OldPostureUp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, PostureUp, OldPostureUp);
}

void UMagicKnightAttributeSet::OnRep_HealingCount(FGameplayAttributeData& OldHealingCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, HealingCount, OldHealingCount);
}

void UMagicKnightAttributeSet::OnRep_MaxHealingCount(FGameplayAttributeData& OldMaxHealingCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxHealingCount, OldMaxHealingCount);
}

void UMagicKnightAttributeSet::OnRep_OldHealing_HP(FGameplayAttributeData& OldHealing_HP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Healing_HP, OldHealing_HP);
}
