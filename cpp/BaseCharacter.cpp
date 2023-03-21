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

	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));//���Ƽ �ý��� ������Ʈ ���� 
	AbilitySystemComponent->SetIsReplicated(true); //���� �������� ����
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); //���� ��Ģ -> �ַΰ��� �ּ�ġ��

	Attribute = CreateDefaultSubobject<UCharacterAttributeSetBase>(TEXT("Attribute")); //��Ʈ����Ʈ ����
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

	//�����Ƽ �ý��� ������Ʈ��, ��Ʈ����Ʈ�� ��ȿ �ϸ�
	if (AbilitySystemComponent && InputComponent)
	{
		//GameData�� ������ EGameAbilityInputID�������� Binds��� ������ ����, Confirm, Cancel�� ���� ���־�� ��
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGameAbilityInputID", static_cast<int32>(EGameAbilityInputID::Confirm), static_cast<int32>(EGameAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds); //Ű����&���콺 �Է°� Binds�� ���� ��
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::InitalizeAttribute() //��Ʈ����Ʈ �ʱ�ȭ
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

void ABaseCharacter::GiveAbilities() //�����Ƽ �ο�
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		//������ �󿡼� ������ DefaultAbilities�� �����Ƽ�� �ο���
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



