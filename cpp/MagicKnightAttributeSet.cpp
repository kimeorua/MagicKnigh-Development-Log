// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicKnightAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

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
