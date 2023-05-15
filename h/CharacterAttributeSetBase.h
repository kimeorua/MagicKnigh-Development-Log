#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSetBase.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MYGAME_API UCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	//���� HP
	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Health)

	//�ִ� HP
	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_EP)
	FGameplayAttributeData EP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, EP)

	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_MaxEP)
	FGameplayAttributeData MaxEP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxEP)

	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_Overload)
	FGameplayAttributeData Overload;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Overload)

	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_MaxOverload)
	FGameplayAttributeData MaxOverload;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxOverload)

	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_Potion)
	FGameplayAttributeData Potion;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Potion)

	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_MaxPotion)
	FGameplayAttributeData MaxPotion;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxPotion)


	//-------------------------------------------Funtion----------------------------------------------------//
	UCharacterAttributeSetBase();
	//�ʼ� ���� �Լ�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const  override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth); // HP ����
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth); //MaxHP ����
	UFUNCTION()
	virtual void OnRep_EP(const FGameplayAttributeData& OldEP); // EP ����
	UFUNCTION()
	virtual void OnRep_MaxEP(const FGameplayAttributeData& OldMaxEP); //MaxEP ����

	UFUNCTION()
	virtual void OnRep_Potion(const FGameplayAttributeData& OldPotion); // ���� ����ȭ ��ġ ����
	UFUNCTION()
	virtual void OnRep_MaxPotion(const FGameplayAttributeData& OldMaxPotion); // �ִ� ������ ��ġ ����

	UFUNCTION()
	virtual void OnRep_Overload(const FGameplayAttributeData& OldOverload); // ���� ���� �� ����
	UFUNCTION()
	virtual void OnRep_MaxOverload(const FGameplayAttributeData& OldMaxOverload); // �ִ� ���� �� ����
};
