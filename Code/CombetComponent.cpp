// Fill out your copyright notice in the Description page of Project Settings.


#include "CombetComponent.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.h"
#include "Weapon.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UCombetComponent::UCombetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TTuple<bool, FHitResult> UCombetComponent::AttackCollision(EAttackCollisionType Type, float Range, EAttackDirectionType DriectionType, ECollisionChannel TraceChannel, bool isPlayer)
{
	FVector Start; // Trace 시작점
	FVector End; // Trace 끝점

	TArray<AActor*> ActorsToIgnore; //무시할 객체
	ActorsToIgnore.Add(GetOwner()); //자기 자신 추가
	FHitResult OutHit; //Hit 결과 구조체
	bool bResult; //충돌 여부

	if (isPlayer)
	{
		AWeapon* Weapon = Cast<APlayerCharacter>(GetOwner())->GetCurrentWeapon();
		switch (DriectionType)
		{
		case EAttackDirectionType::None:
			break;
		case EAttackDirectionType::Left:
			Start = Weapon->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::LeftStartSocket]);
			End = Weapon->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::LeftEndSocket]);
			break;
		case EAttackDirectionType::Right:
			Start = Weapon->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::RightStartSocket]);
			End = Weapon->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::RightEndSocket]);
			break;
		case EAttackDirectionType::AOE:
			Start = Weapon->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::AOE_StartSocket]);
			End = Weapon->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::AOE_EndSocket]);
		case EAttackDirectionType::Max:
			break;
		default:
			break;
		}
	}
	else if (!isPlayer)
	{
		switch (DriectionType)
		{
		case EAttackDirectionType::None:
			break;
		case EAttackDirectionType::Left:
			Start = Cast<ABaseCharacter>(GetOwner())->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::LeftStartSocket]);
			End = Cast<ABaseCharacter>(GetOwner())->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::LeftEndSocket]);
			break;
		case EAttackDirectionType::Right:
			Start = Cast<ABaseCharacter>(GetOwner())->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::RightStartSocket]);
			End = Cast<ABaseCharacter>(GetOwner())->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::RightEndSocket]);
			break;
		case EAttackDirectionType::AOE:
			Start = Cast<ABaseCharacter>(GetOwner())->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::AOE_StartSocket]);
			End = Cast<ABaseCharacter>(GetOwner())->GetMesh()->GetSocketLocation(AttackSockets[EAttackSocket::AOE_EndSocket]);
		case EAttackDirectionType::Max:
			break;
		default:
			break;
		}
	}

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
			UEngineTypes::ConvertToTraceType(TraceChannel),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true);
		break;

	case EAttackCollisionType::AOE:
		bResult = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			Start,
			End,
			Range,
			UEngineTypes::ConvertToTraceType(TraceChannel),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true);
		break;

	case EAttackCollisionType::AOE_Object_Center:
		bResult = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			GetOwner()->GetActorLocation(),
			GetOwner()->GetActorLocation(),
			Range,
			UEngineTypes::ConvertToTraceType(TraceChannel),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true);
		break;

	case EAttackCollisionType::Range_Line:
		bResult = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start,
			TargetLocation,
			UEngineTypes::ConvertToTraceType(TraceChannel),
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

	return MakeTuple(bResult, OutHit);
}

