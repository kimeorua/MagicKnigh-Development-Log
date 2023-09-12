// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MiddleRange_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UBTT_MiddleRange_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MiddleRange_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
