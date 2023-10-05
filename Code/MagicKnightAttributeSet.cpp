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
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetElementalForceAttribute())
	{
		SetElementalForce(FMath::Clamp(GetElementalForce(), 0.f, GetMaxElementalForce()));
	}
	else if (Data.EvaluatedData.Attribute == GetPostureAttribute())
	{
		SetPosture(FMath::Clamp(GetPosture(), 0.f, GetMaxPosture()));
	}
	else if (Data.EvaluatedData.Attribute == GetHealingCountAttribute())
	{
		SetHealingCount(FMath::Clamp(GetHealingCount(), 0.f, GetMaxHealingCount()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - Damage.GetCurrentValue(), 0.f, GetMaxHealth()));
		SetPosture(FMath::Clamp(GetPosture() + Damage.GetCurrentValue() * 1.2f, 0.f, GetMaxPosture()));

		if (GetHealth() <= 0.f)
		{
			SetHealth(0.f);
			ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor());
			Owner->Die();
		}

		else
		{
			if (GetPosture() >= GetMaxPosture())
			{
				SetPosture(0.f);
				ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor());
				//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Owner->GetName())
				Owner->Stun();
			}
		}

		Damage = 0.f;
	}
	else if (Data.EvaluatedData.Attribute == GetChargeEFAttribute())
	{
		SetElementalForce(FMath::Clamp(GetElementalForce() + ChargeEF.GetCurrentValue(), 0, GetMaxElementalForce()));
		ChargeEF = 0.f;
	}
	else if (Data.EvaluatedData.Attribute == GetPostureUpAttribute())
	{
		SetPosture(FMath::Clamp(GetPosture() + PostureUp.GetCurrentValue(), 0, GetMaxPosture()));
		if (GetPosture() >= GetMaxPosture())
		{
			SetPosture(0.f);
			ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor());
			//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Owner->GetName())
			Owner->Stun();
		}
		PostureUp = 0.f;
	}
	else if (Data.EvaluatedData.Attribute == GetHealing_HPAttribute())
	{
		if (GetHealth() < GetMaxHealth() && GetHealingCount() > 0)
		{
			SetHealingCount(GetHealingCount() - 1);
			SetHealth(FMath::Clamp(GetHealth(), GetHealth() + Healing_HP.GetCurrentValue(), GetMaxHealth()));
			UE_LOG(LogTemp, Warning, TEXT("Heal"));
		}
		SetHealing_HP(0.f);
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
