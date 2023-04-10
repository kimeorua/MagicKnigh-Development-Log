﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "InAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UInAttackRange : public UBTService
{
	GENERATED_BODY()
public:
	UInAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
