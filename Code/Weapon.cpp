// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"
#include "Math/Rotator.h"
#include "Engine/EngineTypes.h"
#include "Abilities/GameplayAbility.h"
#include "EnemyCharacter.h"
#include "CombetComponent.h"

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

FHitResult AWeapon::CheakCollision(EAttackCollisionType Type, float Range, EAttackDirectionType DriectionType,  EDamageEffectType DamgeType)
{
	TTuple<bool, FHitResult> CollisionResult = Cast<APlayerCharacter>(GetOwner())->GetCombetComponent()->AttackCollision(Type, Range, DriectionType, ECollisionChannel::ECC_GameTraceChannel2, true);
	bool isHit = CollisionResult.Get<0>(); 
	FHitResult OutHit = CollisionResult.Get<1>(); 

	if (isHit)
	{
		if (!Cast<AEnemyCharacter>(OutHit.GetActor())->bIsDie)
		{
			if (HitEnemys.IsEmpty())
			{
				HitEnemys.Add(Cast<AEnemyCharacter>(OutHit.GetActor()));
				HitEnemys[0]->TakeDamgeFormPlayer(DamgeType);
				APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
				if (IsValid(Player)) { Player->EFCharge(); }
			}
			else if (!HitEnemys.IsEmpty())
			{
				if (!HitEnemys.Contains(OutHit.GetActor()))
				{
					AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(OutHit.GetActor());
					HitEnemy->TakeDamgeFormPlayer(DamgeType);
					HitEnemys.Add(HitEnemy);
				}
			}
		}
	}
	return OutHit;
}

void AWeapon::HitArrReset()
{
	HitEnemys.Empty();
}

