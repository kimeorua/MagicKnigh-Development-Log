	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SettingPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PF_MAGICKNIGHT_API UBTT_SettingPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SettingPatrolPoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
