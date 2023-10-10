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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent; //어빌리티 시스템 컴포넌트

	UPROPERTY()
	UMagicKnightAttributeSet* Attributes; //어트리뷰트

protected:
	FTimerHandle PostureHandle;
	//사용하는 전투 용 이펙트들(EX: 슈퍼아머, 체간 상승, 방어, 페링)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TMap<ECombetEffectType, TSubclassOf<class UGameplayEffect>> CombetEffects;

	//사용하는 데미지 용 이펙트(체력 감소)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TMap<EDamageEffectType, TSubclassOf<class UGameplayEffect>> DamageEffects;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect; //기본 스텟 설정용 GameplayEffect

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TArray<TSubclassOf<class UGameplayAbility>>DefaultAbility; //사용하는 GameplayAbility

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bIsStun = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bIsDie = false;

	//체간 감소 타이머 작동 시간 -> 테스트를 위해 블루프린트에서 설정 가능 하도록 구현
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parrying", meta = (AllowPrivateAccess = "true"))
	float InDelayTime_Posture = 1.f;

//----------------------------------------------------함수----------------------------------------------------//
public:
	// Sets default values for this character's properties
	ABaseCharacter();

	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; };
	FORCEINLINE UMagicKnightAttributeSet* GetMagicKnightAttributeSet() const { return Attributes; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override; //캐릭터가 빙의 되었을때 호출 됨
	virtual void OnRep_PlayerState() override; //복제 알림(어빌리티는 기본적으로 복제되어 사용됨)
	virtual void InitalizeAttribute(); //어트리뷰트 초기화
	virtual void GiveAbilities(); //어빌리티 부여

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//기절
	void Stun();

	//기절 종료
	UFUNCTION(BlueprintCallable)
	void StunEnd();

	//사망
	virtual void Die();

	void DecreasePosture();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ReStartPostureTimer() const { GetWorldTimerManager().UnPauseTimer(PostureHandle); }
};
