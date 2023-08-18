// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

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
	StartPoint = GetActorLocation();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[0], 1, EffectContext);

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

TArray<FVector> AEnemyCharacter::GetPatrolPoints() const
{
	TArray<FVector>PatrolPointsArr;
	for (ATargetPoint* Point : PatrolPoints)
	{
		PatrolPointsArr.Add(Point->GetActorLocation());
	}
	return PatrolPointsArr;
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