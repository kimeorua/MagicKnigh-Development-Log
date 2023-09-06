// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include <GameplayEffectTypes.h>
#include "AbilitySystemInterface.h"
#include "MagicKnightAttributeSet.h"
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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect; //기본 스텟 설정용 GameplayEffect

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TArray<TSubclassOf<class UGameplayAbility>>DefaultAbility; //사용하는 GameplayAbility

//----------------------------------------------------함수----------------------------------------------------//
public:
	// Sets default values for this character's properties
	ABaseCharacter();

	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; };

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
};
