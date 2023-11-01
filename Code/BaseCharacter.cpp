// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "MagicKnightAttributeSet.h"
#include "CombetComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));//���Ƽ �ý��� ������Ʈ ���� 
	AbilitySystemComponent->SetIsReplicated(true); //���� �������� ����
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); //���� ��Ģ -> �ַΰ��� �ּ�ġ��

	Attributes = CreateDefaultSubobject<UMagicKnightAttributeSet>(TEXT("Attributes")); //��Ʈ����Ʈ ����

	CombetComponent = CreateDefaultSubobject<UCombetComponent>(TEXT("Combet Component"));
}

//���ǽ� ȣ��
void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	InitalizeAttribute();
	GiveAbilities();
}

//���� �˸�(�����Ƽ�� �⺻������ �����Ǿ� ����)
void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	InitalizeAttribute();
}

//��Ʈ����Ʈ �ʱ�ȭ
void ABaseCharacter::InitalizeAttribute()
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

//�����Ƽ �ο�
void ABaseCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& StartupAbility : DefaultAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility.GetDefaultObject(), 1, 0));
		}
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//----------------------------------------------------ü�� ����-----------------------------------------------------//
	GetWorldTimerManager().SetTimer(PostureHandle, this, &ABaseCharacter::DecreasePosture, InDelayTime_Posture, true);
	//----------------------------------------------------ü�� ����-----------------------------------------------------//
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

}

//���� �Լ� -> Attriute���� ȣ��.
void ABaseCharacter::Stun()
{
	bIsStun = true;
	//UE_LOG(LogTemp, Warning, TEXT("Base Stun"));
}

// ���� ���� -> ���ϸ��̼ǿ��� ȣ��
void ABaseCharacter::StunEnd()
{
	bIsStun = false;
}

//��� -> Attribute���� ȣ��
void ABaseCharacter::Die()
{
	bIsDie = true;
}

void ABaseCharacter::DecreasePosture()
{
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle PostureSpecHandle;

	if (GetMagicKnightAttributeSet()->GetPosture() > 0.f) //ü���� 0���� GameplayEffect ���� 
	{
		PostureSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(CombetEffects[ECombetEffectType::DecreasePosture], 1, EffectContext);
	}

	if (PostureSpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle Posture_GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*PostureSpecHandle.Data.Get());
	}
}