// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"
#include "Math/Rotator.h"
#include "Engine/EngineTypes.h"
#include "Abilities/GameplayAbility.h"
#include "EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	HitEnemys.Empty();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FHitResult AWeapon::CheakCollision(EAttackCollisionType Type, float Range = 0.f)
{
	FVector Start = Mesh->GetSocketLocation(CollisionStartSocket); //시작 점
	FVector End = Mesh->GetSocketLocation(CollisionEndSocket); //끝 점
	FVector AOE = Mesh->GetSocketLocation(CollisionAOESocket); //범위 스킬 끝점

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
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true);
		break;

	case EAttackCollisionType::AOE:
		bResult = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			End,
			AOE,
			Range,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration, 
			OutHit,
			true);
		break;

	case EAttackCollisionType::AOE_Player_Center:
		bResult = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			GetOwner()->GetActorLocation(),
			GetOwner()->GetActorLocation(),
			Range,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true);
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
		if (HitEnemys.IsEmpty())
		{
			HitEnemys.Add(Cast<AEnemyCharacter>(OutHit.GetActor()));
			HitEnemys[0]->TakeDamgeFormPlayer();
			Cast<APlayerCharacter>(GetOwner())->EFCharge();
		}
		else if(!HitEnemys.IsEmpty())
		{
			if (!HitEnemys.Contains(OutHit.GetActor()))
			{
				AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(OutHit.GetActor());
				HitEnemy->TakeDamgeFormPlayer();
				HitEnemys.Add(HitEnemy);
			}
		}
	}
	return OutHit;
}

void AWeapon::HitArrReset()
{
	HitEnemys.Empty();
}

