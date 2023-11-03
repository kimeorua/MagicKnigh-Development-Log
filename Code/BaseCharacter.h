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

//---------------------------------------------------변수----------------------------------------------------//
private:
	//어빌리티 시스템 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	//어트리뷰트
	UPROPERTY()
	UMagicKnightAttributeSet* Attributes;

protected:
	//체간 감소 작동 타이머
	FTimerHandle PostureHandle;

	//사용하는 전투 용 이펙트들(EX: 슈퍼아머, 체간 상승, 방어, 페링)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TMap<ECombetEffectType, TSubclassOf<class UGameplayEffect>> CombetEffects;

	//사용하는 데미지 용 이펙트(체력 감소)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TMap<EDamageEffectType, TSubclassOf<class UGameplayEffect>> DamageEffects;

public:
	//기본 스텟 설정용 GameplayEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect; 

	//사용하는 GameplayAbility
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TArray<TSubclassOf<class UGameplayAbility>>DefaultAbility; 

	//기절 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bIsStun = false;

	//사망 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bIsDie = false;

	//체간 감소 타이머 작동 시간 -> 테스트를 위해 블루프린트에서 설정 가능 하도록 구현
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parrying", meta = (AllowPrivateAccess = "true"))
	float InDelayTime_Posture = 1.f;

	// 공격 오버랩 판정용 전투 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombetComponent", meta = (AllowPrivateAccess = "true"))
	class UCombetComponent* CombetComponent;

//----------------------------------------------------함수----------------------------------------------------//
public:
	// Sets default values for this character's properties
	ABaseCharacter();

	/// <summary>
	/// 어빌리티 시스템 컴포넌트 반환
	/// </summary>
	/// <returns>AbilitySystemComponent</returns>
	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; };

	/// <summary>
	/// 어트리뷰트 셋 반환
	/// </summary>
	/// <returns>Attributes</returns>
	FORCEINLINE UMagicKnightAttributeSet* GetMagicKnightAttributeSet() const { return Attributes; }

	/// <summary>
	/// 전투 컴포넌트 반환
	/// </summary>
	/// <returns>CombetComponent</returns>
	FORCEINLINE UCombetComponent* GetCombetComponent() const { return CombetComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// 컨트롤러 빙의시 호출
	/// </summary>
	/// <param name="NewController">컨트롤러</param>
	virtual void PossessedBy(AController* NewController) override;

	/// <summary>
	/// 복제 알림(어빌리티는 기본적으로 복제되어 사용됨)
	/// </summary>
	virtual void OnRep_PlayerState() override;

	/// <summary>
	/// 어트리뷰트 초기화
	/// </summary>
	virtual void InitalizeAttribute(); 

	/// <summary>
	/// 어빌리티 부여
	/// </summary>
	virtual void GiveAbilities();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/// <summary>
	/// 기절 작동
	/// </summary>
	void Stun();

	/// <summary>
	/// 기절 종료
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void StunEnd();

	/// <summary>
	/// 사망 작동
	/// </summary>
	virtual void Die();

	/// <summary>
	/// 체간 감소 작동
	/// </summary>
	void DecreasePosture();

	/// <summary>
	/// 체간 감소 작동 재 작동
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ReStartPostureTimer() const { GetWorldTimerManager().UnPauseTimer(PostureHandle); }
};
