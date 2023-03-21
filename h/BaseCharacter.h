// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameData.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class UCharacterGameplayAbility;
class UCharacterAttributeSetBase;
class UCharacterAbilitySystemComponent;

UCLASS()
class MYGAME_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface // �����Ƽ�ý��� �������̽� ���
{
	GENERATED_BODY()

private:
	//�����Ƽ �ý��� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCharacterAbilitySystemComponent* AbilitySystemComponent;

	//��Ʈ����Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterAttributeSetBase>Attribute;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // �����Ƽ �ý��� ������Ʈ ��ȯ�� �Լ�

	virtual void InitalizeAttribute(); //��Ʈ����Ʈ �ʱ�ȭ
	virtual void GiveAbilities(); //�����Ƽ �ο�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect>DefaultAttributeEffect; //������ �󿡼� ������ �⺻ �����Ƽ ����Ʈ(HP, MaxHP�� ���� ����)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UCharacterGameplayAbility>>DefaultAbilities; //ĳ���Ͱ� ����� �����Ƽ(�ɷ�) EX)ȸ��, ��� etc....


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override; //ĳ���Ͱ� ���� �Ǿ����� ȣ�� ��
	virtual void OnRep_PlayerState() override; //���� �˸�(�����Ƽ�� �⺻������ �����Ǿ� ����)
};
