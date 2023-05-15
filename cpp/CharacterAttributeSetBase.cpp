// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"

UCharacterAttributeSetBase::UCharacterAttributeSetBase()
{
}

void UCharacterAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, EP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxEP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, Overload, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxOverload, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, Potion, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxPotion, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, Health, OldHealth);
}

void UCharacterAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSetBase::OnRep_EP(const FGameplayAttributeData& OldEP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, EP, OldEP);
}

void UCharacterAttributeSetBase::OnRep_MaxEP(const FGameplayAttributeData& OldMaxEP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxEP, OldMaxEP);
}

void UCharacterAttributeSetBase::OnRep_Potion(const FGameplayAttributeData& OldPotion)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, Potion, OldPotion);
}

void UCharacterAttributeSetBase::OnRep_MaxPotion(const FGameplayAttributeData& OldMaxPotion)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxPotion, OldMaxPotion);
}

void UCharacterAttributeSetBase::OnRep_Overload(const FGameplayAttributeData& OldOverload)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, Overload, OldOverload);
}

void UCharacterAttributeSetBase::OnRep_MaxOverload(const FGameplayAttributeData& OldMaxOverload)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxOverload, OldMaxOverload);
}
