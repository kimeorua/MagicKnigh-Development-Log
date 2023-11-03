// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include <GameplayEffectTypes.h>
#include "AbilitySystemInterface.h"
#include "MagicKnightAttributeSet.h"
#include "MagicKnightEnums.h"
#include "BaseCharacter.generated.h"

UCLASS(config = Game)
class PF_MAGICKNIGHT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

//---------------------------------------------------����----------------------------------------------------//
private:
	//�����Ƽ �ý��� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	//��Ʈ����Ʈ
	UPROPERTY()
	UMagicKnightAttributeSet* Attributes;

protected:
	//ü�� ���� �۵� Ÿ�̸�
	FTimerHandle PostureHandle;

	//����ϴ� ���� �� ����Ʈ��(EX: ���۾Ƹ�, ü�� ���, ���, �丵)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TMap<ECombetEffectType, TSubclassOf<class UGameplayEffect>> CombetEffects;

	//����ϴ� ������ �� ����Ʈ(ü�� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TMap<EDamageEffectType, TSubclassOf<class UGameplayEffect>> DamageEffects;

public:
	//�⺻ ���� ������ GameplayEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect; 

	//����ϴ� GameplayAbility
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TArray<TSubclassOf<class UGameplayAbility>>DefaultAbility; 

	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bIsStun = false;

	//��� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bIsDie = false;

	//ü�� ���� Ÿ�̸� �۵� �ð� -> �׽�Ʈ�� ���� �������Ʈ���� ���� ���� �ϵ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parrying", meta = (AllowPrivateAccess = "true"))
	float InDelayTime_Posture = 1.f;

	// ���� ������ ������ ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombetComponent", meta = (AllowPrivateAccess = "true"))
	class UCombetComponent* CombetComponent;

//----------------------------------------------------�Լ�----------------------------------------------------//
public:
	// Sets default values for this character's properties
	ABaseCharacter();

	/// <summary>
	/// �����Ƽ �ý��� ������Ʈ ��ȯ
	/// </summary>
	/// <returns>AbilitySystemComponent</returns>
	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; };

	/// <summary>
	/// ��Ʈ����Ʈ �� ��ȯ
	/// </summary>
	/// <returns>Attributes</returns>
	FORCEINLINE UMagicKnightAttributeSet* GetMagicKnightAttributeSet() const { return Attributes; }

	/// <summary>
	/// ���� ������Ʈ ��ȯ
	/// </summary>
	/// <returns>CombetComponent</returns>
	FORCEINLINE UCombetComponent* GetCombetComponent() const { return CombetComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// ��Ʈ�ѷ� ���ǽ� ȣ��
	/// </summary>
	/// <param name="NewController">��Ʈ�ѷ�</param>
	virtual void PossessedBy(AController* NewController) override;

	/// <summary>
	/// ���� �˸�(�����Ƽ�� �⺻������ �����Ǿ� ����)
	/// </summary>
	virtual void OnRep_PlayerState() override;

	/// <summary>
	/// ��Ʈ����Ʈ �ʱ�ȭ
	/// </summary>
	virtual void InitalizeAttribute(); 

	/// <summary>
	/// �����Ƽ �ο�
	/// </summary>
	virtual void GiveAbilities();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/// <summary>
	/// ���� �۵�
	/// </summary>
	void Stun();

	/// <summary>
	/// ���� ����
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void StunEnd();

	/// <summary>
	/// ��� �۵�
	/// </summary>
	virtual void Die();

	/// <summary>
	/// ü�� ���� �۵�
	/// </summary>
	void DecreasePosture();

	/// <summary>
	/// ü�� ���� �۵� �� �۵�
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ReStartPostureTimer() const { GetWorldTimerManager().UnPauseTimer(PostureHandle); }
};
