// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.h"
#include "EnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
	Controller = nullptr;

	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);
	HitCollision->SetCollisionProfileName(TEXT("EnemyHit"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	Controller = Cast<AEnemyAIController>(GetController());
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	EnemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance()); //애니메이션 호출을 위한 변수
	EnemyAnim->OnMontageEnded.AddDynamic(this, &AEnemyCharacter::OnAttackMontageEnded);
}

void AEnemyCharacter::FindPlayer()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AEnemyCharacter::LosePlayer()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AEnemyCharacter::TakeDamgeFormPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle;

	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.SuperArmor"))) > 0)
	{
		if (IsValid(HitSound) && HitParticle != nullptr)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetMesh()->GetSocketLocation(HitParticleSocket));
		}
		SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[3], 1, EffectContext);
	}
	else
	{
		SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[0], 1, EffectContext);
	}

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

FHitResult AEnemyCharacter::CheakCollision(EAttackCollisionType Type, float Range)
{
	FVector Start = GetMesh()->GetSocketLocation(CollisionStartSocket); //시작 점
	FVector End = GetMesh()->GetSocketLocation(CollisionEndSocket); //끝 점

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	FHitResult OutHit;
	bool bResult;

	switch (Type)
	{
	case EAttackCollisionType::None:
		bResult = false;
		break;

	case EAttackCollisionType::Melee:
		bResult = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel3),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true);
		break;

	case EAttackCollisionType::AOE:
		bResult = false;
		break;

	case EAttackCollisionType::AOE_Player_Center:
		bResult = false;
		break;

	case EAttackCollisionType::Max:
		bResult = false;
		break;

	default:
		bResult = false;
		break;
	}

	if (bResult)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player_Hit"));

		APlayerCharacter* Player = Cast<APlayerCharacter>(OutHit.GetActor());
		if (Player && !PlayerIsHit)
		{
			Player->SetInstigator(this);
			Player->TakeDamageFromEnemy();
			PlayerIsHit = true;
		}
	}

	return OutHit;
}

void AEnemyCharacter::PlayerHitReset()
{
	PlayerIsHit = false;
}

void AEnemyCharacter::TakeParrying()
{
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle;

	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.Parryable"))) > 0)
	{
		SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[2], 1, EffectContext);
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Enemy.State.Parryable")));
	}
	else
	{
		SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[1], 1, EffectContext);
	}

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AEnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast(); //BTTask_EnemyAttack에서 선언한 공격 종료 함수가 호출되도록 델리게이트를 이용하여 구축
}
