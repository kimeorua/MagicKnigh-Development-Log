// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MagicKnightEnums.h"
#include "CombetComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackSocket : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	LeftStartSocket UMETA(DisplayName = "Left Start Socket"), //�޼� ����
	LeftEndSocket UMETA(DisplayName = "Left End Socket"), //�޼� ��
	RightStartSocket UMETA(DisplayName = "Right Start Socket"), //������ ����
	RightEndSocket UMETA(DisplayName = "Right End Socket"), //������ ��
	AOE_StartSocket UMETA(DisplayName = "AOE Start Socket"), //���� ���� ����
	AOE_EndSocket UMETA(DisplayName = "AOE End Socket"), //���� ���� ��

	Max UMETA(DisplayName = "Max")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PF_MAGICKNIGHT_API UCombetComponent : public UActorComponent
{
	GENERATED_BODY() 

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	TMap<EAttackSocket, FName> AttackSockets;

	FVector TargetLocation;

public:	
	// Sets default values for this component's properties
	UCombetComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TTuple<bool, FHitResult> AttackCollision(EAttackCollisionType Type, float Range, EAttackDirectionType DriectionType, ECollisionChannel TraceChannel, bool isPlayer = false);

	void SetTargetLocation(FVector TLocation) { TargetLocation = TLocation; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	
};
