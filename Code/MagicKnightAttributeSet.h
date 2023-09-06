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

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data); // �Ӽ��� �⺻ ���� �����ϱ� ���� GameplayEffect�� ����Ǳ� ������ ȣ��˴ϴ�

	//-------------------------------------����--------------------------------------------------//

	// ����� (HP)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Health)
	
	// �ִ� �����(MaxHP)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth = 100.f;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxHealth)

	//���ҷ�(EF) -> RPG�� ������ ���
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData  Posture;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Posture)

	//�ִ� ���ҷ�(MaxEF)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxPosture = 100.f;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxPosture)

	//���ҷ�(EF) -> RPG�� ������ ���
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ElementalForce;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, ElementalForce)

	//�ִ� ���ҷ�(MaxEF)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxElementalForce = 100.f;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, MaxElementalForce)

	//���� ������(Damage)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, Damage)

	//�÷��̾ ���� �� ȹ���ϴ� ���ҷ�(ChargeEF)
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChargeEF;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, ChargeEF)

	//�ǰ� �� ��� ü��
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData PostureUp;
	ATTRIBUTE_ACCESSORS(UMagicKnightAttributeSet, PostureUp)

};
