#### UMagicKnightAttributeSet.h
```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MagicKnightAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UMagicKnightAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	//-------------------------------------함수--------------------------------------------------//
	UFUNCTION()
	virtual void OnRep_Damage(FGameplayAttributeData& OldDamage);

	UFUNCTION()
	virtual void OnRep_ChargeEF(FGameplayAttributeData& OldChargeEF);

	UFUNCTION()
	virtual void OnRep_PostureUp(FGameplayAttributeData& OldPostureUp);

	//-------------------------------------변수--------------------------------------------------//

	//받은 데미지(Damage)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Damage)

	//플레이어가 공격 시 획득하는 원소력(ChargeEF)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChargeEF;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, ChargeEF)

	//피격 시 얻는 체간
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData PostureUp;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, PostureUp)

};
```

#### MagicKnightAttributeSet.cpp

```cpp

// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicKnightAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UMagicKnightAttributeSet::UMagicKnightAttributeSet()
{

}

void UMagicKnightAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute()) //현제 변경된 값이, Damage 일경우(피격)
	{
		SetHealth(FMath::Clamp(GetHealth() - Damage.GetCurrentValue(), 0.f, GetMaxHealth())); //HP - Damge를 계산하고
		SetPosture(FMath::Clamp(GetPosture() + Damage.GetCurrentValue() * 1.2f, 0.f, GetMaxPosture())); //체간을 Damage * 1.2 만큼 상승
		Damage = 0.f; //Damage 초기화
	}
	else if (Data.EvaluatedData.Attribute == GetChargeEFAttribute()) //현제 변경된 값이 ChargeEF일 경우(공격에 성공, 플레이어만 해당)
	{
		SetElementalForce(FMath::Clamp(GetElementalForce() + ChargeEF.GetCurrentValue(), 0, GetMaxElementalForce())); //EF를 ChargeEF만큼 충전
		ChargeEF = 0.f; //ChargeEF 초기화
	}
	else if (Data.EvaluatedData.Attribute == GetPostureUpAttribute())// 현제 변경된 값이 PostureUp일 경우(방어 성공)
	{
		SetPosture(FMath::Clamp(GetPosture() + PostureUp.GetCurrentValue(), 0, GetMaxPosture())); // 체간 상승
		PostureUp = 0.f; //초기화
	}
}

void UMagicKnightAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, ChargeEF, COND_None, REPNOTIFY_Always);
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
