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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const  override; //네트워크 복제에 사용되는 속성을 반환합니다. 기본 복제 속성이 있는 모든 액터 클래스에서 재정의해야 합니다.

	UFUNCTION()
	virtual void OnRep_Health(FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_ElementalForce(FGameplayAttributeData& OldElementalForce);

	UFUNCTION()
	virtual void OnRep_MaxElementalForce(FGameplayAttributeData& OldMaxElementalForce);


	//-------------------------------------변수--------------------------------------------------//

	// 생명력 (HP)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Health)
	
	// 최대 생명력(MaxHP)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxHealth)

	//원소력(EF) -> RPG의 마나와 비슷
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ElementalForce;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, ElementalForce)

	//최대 원소력(MaxEF)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxElementalForce;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxElementalForce)
};
