// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PatrolPointUp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PF_MAGICKNIGHT_API UBTT_PatrolPointUp : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_PatrolPointUp();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
