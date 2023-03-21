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
class MYGAME_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface // 어빌리티시스템 인터페이스 상속
{
	GENERATED_BODY()

private:
	//어빌리티 시스템 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCharacterAbilitySystemComponent* AbilitySystemComponent;

	//어트리뷰트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterAttributeSetBase>Attribute;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // 어빌리티 시스템 컴포넌트 반환용 함수

	virtual void InitalizeAttribute(); //어트리뷰트 초기화
	virtual void GiveAbilities(); //어빌리티 부여

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect>DefaultAttributeEffect; //에디터 상에서 설정할 기본 어빌리티 이펙트(HP, MaxHP등 스텟 설정)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UCharacterGameplayAbility>>DefaultAbilities; //캐릭터가 사용할 어빌리티(능력) EX)회피, 방어 etc....


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override; //캐릭터가 빙의 되었을때 호출 됨
	virtual void OnRep_PlayerState() override; //복제 알림(어빌리티는 기본적으로 복제되어 사용됨)
};
