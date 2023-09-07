// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BTAuxiliaryNode.h"
#include "BTS_RandomPattern.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UBTS_RandomPattern : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_RandomPattern();
protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
