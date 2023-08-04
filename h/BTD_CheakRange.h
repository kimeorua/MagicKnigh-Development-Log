// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheakRange.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API UBTD_CheakRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CheakRange();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance")
	float Distance; //에디터 상에서 지정할 거리 최대 550

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
