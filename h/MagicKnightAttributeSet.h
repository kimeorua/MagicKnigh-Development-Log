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
	//-------------------------------------�Լ�--------------------------------------------------//

	UMagicKnightAttributeSet(); //������
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const  override; //��Ʈ��ũ ������ ���Ǵ� �Ӽ��� ��ȯ�մϴ�. �⺻ ���� �Ӽ��� �ִ� ��� ���� Ŭ�������� �������ؾ� �մϴ�.

	UFUNCTION()
	virtual void OnRep_Health(FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_ElementalForce(FGameplayAttributeData& OldElementalForce);

	UFUNCTION()
	virtual void OnRep_MaxElementalForce(FGameplayAttributeData& OldMaxElementalForce);


	//-------------------------------------����--------------------------------------------------//

	// ����� (HP)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Health)
	
	// �ִ� �����(MaxHP)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxHealth)

	//���ҷ�(EF) -> RPG�� ������ ���
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ElementalForce;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, ElementalForce)

	//�ִ� ���ҷ�(MaxEF)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxElementalForce;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxElementalForce)
};
