// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "CharacterAbilitySystemComponent.h"
#include "CharacterAttributeSetBase.h"
#include "CharacterGameplayAbility.h"
#include "GameplayEffectTypes.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));//어비리티 시스템 컴포넌트 생성 
	AbilitySystemComponent->SetIsReplicated(true); //복제 가능으로 설정
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); //복제 규칙 -> 솔로게임 최소치로

	Attribute = CreateDefaultSubobject<UCharacterAttributeSetBase>(TEXT("Attribute")); //어트리뷰트 생성
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//어빌리티 시스템 컴포넌트와, 어트리뷰트가 유효 하면
	if (AbilitySystemComponent && InputComponent)
	{
		//GameData에 지정한 EGameAbilityInputID열거형을 Binds라는 변수에 저장, Confirm, Cancel은 지정 해주어야 함
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGameAbilityInputID", static_cast<int32>(EGameAbilityInputID::Confirm), static_cast<int32>(EGameAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds); //키보드&마우스 입력과 Binds를 연결 함
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::InitalizeAttribute() //어트리뷰트 초기화
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ABaseCharacter::GiveAbilities() //어빌리티 부여
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		//에디터 상에서 설정한 DefaultAbilities를 어빌리티를 부여함
		for (TSubclassOf<UCharacterGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitalizeAttribute();
	GiveAbilities();
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitalizeAttribute();
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGameAbilityInputID", static_cast<int32>(EGameAbilityInputID::Confirm), static_cast<int32>(EGameAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}



