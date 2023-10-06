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

	UMagicKnightAttributeSet(); //생성자
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const  override; //네트워크 복제에 사용되는 속성을 반환. 기본 복제 속성이 있는 모든 액터 클래스에서 재정의 해야 함ㄴ

	UFUNCTION()
	virtual void OnRep_Health(FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Posture(FGameplayAttributeData& OldPosture);

	UFUNCTION()
	virtual void OnRep_MaxPosture(FGameplayAttributeData& OldMaxPosture);

	UFUNCTION()
	virtual void OnRep_ElementalForce(FGameplayAttributeData& OldElementalForce);

	UFUNCTION()
	virtual void OnRep_MaxElementalForce(FGameplayAttributeData& OldMaxElementalForce);

	UFUNCTION()
	virtual void OnRep_Damage(FGameplayAttributeData& OldDamage);

	UFUNCTION()
	virtual void OnRep_ChargeEF(FGameplayAttributeData& OldChargeEF);

	UFUNCTION()
	virtual void OnRep_PostureUp(FGameplayAttributeData& OldPostureUp);

	UFUNCTION()
	virtual void OnRep_HealingCount(FGameplayAttributeData& OldHealingCount);

	UFUNCTION()
	virtual void OnRep_MaxHealingCount(FGameplayAttributeData& OldMaxHealingCount);

	UFUNCTION()
	virtual void OnRep_OldHealing_HP(FGameplayAttributeData& OldHealing_HP);

	// 속성의 기본 값을 수정하기 위해 GameplayEffect가 실행되기 직전에 호출
	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data); 

	//-------------------------------------변수--------------------------------------------------//

	// 생명력 (HP)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Health)
	
	// 최대 생명력(MaxHP)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth = 100.f;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxHealth)

	//원소력(EF) -> RPG의 마나와 비슷
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData  Posture;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Posture)

	//최대 원소력(MaxEF)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxPosture = 100.f;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxPosture)

	//원소력(EF) -> RPG의 마나와 비슷
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ElementalForce;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, ElementalForce)

	//최대 원소력(MaxEF)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxElementalForce = 100.f;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxElementalForce)

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

	//남은 회복 스킬 사용 횟수
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData HealingCount;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, HealingCount)
	
	//최대 회복 스킬 사용 횟수
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealingCount = 5.0f;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxHealingCount)

	// 회복되는 체력 량
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Healing_HP;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Healing_HP)
};
