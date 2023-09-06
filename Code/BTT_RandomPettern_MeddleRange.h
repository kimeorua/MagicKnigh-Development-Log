// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RandomPettern_MeddleRange.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UBTT_RandomPettern_MeddleRange : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_RandomPettern_MeddleRange();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
