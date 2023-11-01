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

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));//어비리티 시스템 컴포넌트 생성 
	AbilitySystemComponent->SetIsReplicated(true); //복제 가능으로 설정
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); //복제 규칙 -> 솔로게임 최소치로

	Attributes = CreateDefaultSubobject<UMagicKnightAttributeSet>(TEXT("Attributes")); //어트리뷰트 생성

	CombetComponent = CreateDefaultSubobject<UCombetComponent>(TEXT("Combet Component"));
}

//빙의시 호출
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

//복제 알림(어빌리티는 기본적으로 복제되어 사용됨)
void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	InitalizeAttribute();
}

//어트리뷰트 초기화
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

//어빌리티 부여
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

	//----------------------------------------------------체간 감소-----------------------------------------------------//
	GetWorldTimerManager().SetTimer(PostureHandle, this, &ABaseCharacter::DecreasePosture, InDelayTime_Posture, true);
	//----------------------------------------------------체간 감소-----------------------------------------------------//
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

//기절 함수 -> Attriute에서 호출.
void ABaseCharacter::Stun()
{
	bIsStun = true;
	//UE_LOG(LogTemp, Warning, TEXT("Base Stun"));
}

// 기절 종료 -> 에니메이션에서 호출
void ABaseCharacter::StunEnd()
{
	bIsStun = false;
}

//사망 -> Attribute에서 호출
void ABaseCharacter::Die()
{
	bIsDie = true;
}

void ABaseCharacter::DecreasePosture()
{
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle PostureSpecHandle;

	if (GetMagicKnightAttributeSet()->GetPosture() > 0.f) //체간이 0보다 GameplayEffect 설정 
	{
		PostureSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(CombetEffects[ECombetEffectType::DecreasePosture], 1, EffectContext);
	}

	if (PostureSpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle Posture_GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*PostureSpecHandle.Data.Get());
	}
}