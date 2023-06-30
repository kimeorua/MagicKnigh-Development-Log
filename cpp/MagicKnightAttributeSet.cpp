// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicKnightAttributeSet.h"
#include "Net/UnrealNetwork.h"

UMagicKnightAttributeSet::UMagicKnightAttributeSet()
{

}

void UMagicKnightAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, Health, COND_None,  REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, ElementalForce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxElementalForce, COND_None, REPNOTIFY_Always);
}

void UMagicKnightAttributeSet::OnRep_Health(FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Health, OldHealth);
}

void UMagicKnightAttributeSet::OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxHealth, OldMaxHealth);
}

void UMagicKnightAttributeSet::OnRep_ElementalForce(FGameplayAttributeData& OldElementalForce)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, ElementalForce, OldElementalForce);
}

void UMagicKnightAttributeSet::OnRep_MaxElementalForce(FGameplayAttributeData& OldMaxElementalForce)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxElementalForce, OldMaxElementalForce);
}
