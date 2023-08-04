// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_DistanceToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UBTS_DistanceToPlayer : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_DistanceToPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float Distance = 600.f;
};
