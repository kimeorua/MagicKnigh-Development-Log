// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_InCloseAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UBTD_InCloseAttackRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTD_InCloseAttackRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
