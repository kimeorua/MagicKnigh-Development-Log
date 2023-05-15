// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameData.h"
#include "EnemyAnimInstance.h"
#include "EnemyController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemy::AEnemy()
{
	EnemyAnimInstance = nullptr;
	MeleePatternMontage = nullptr;
	MeleePattern2Montage = nullptr;
	ComboNum = 0;
	CurrentComboAttackMontage = nullptr;
	Controller = nullptr;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	Controller = Cast<AEnemyController>(GetController());
	Controller->GetBlackboardComponent()->SetValueAsEnum(Controller->AttackPattern, 1);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::ComboUp()
{
	ComboNum ++;

	if (ComboNum > 2)
	{
		ComboNum = 0;
	}
}

UAnimMontage* AEnemy::GetCurrentCombo()
{
	switch (ComboNum)
	{
	case 0:
	{
		CurrentComboAttackMontage = ComboAttackMontage[0];
		break;
	}
	case 1:
	{
		CurrentComboAttackMontage = ComboAttackMontage[1];
		break;
	}
	case 2:
	{
		CurrentComboAttackMontage = ComboAttackMontage[2];
		break;
	}
	default:
		break;
	}
	return CurrentComboAttackMontage;
}

void AEnemy::RandomPattern()
{
	int x = 0;	//랜덤한 숫자를 저장할 변수
	x = FMath::RandRange(1, 100);		// 랜덤 값 저장

	if (x <= 25)
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum(Controller->AttackPattern, 1);
	}
	else if (x <= 65)
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum(Controller->AttackPattern, 2);
	}
	else if (x <= 100)
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum(Controller->AttackPattern, 3);
	}
}

void AEnemy::Die()
{
	Controller->GetBehaviorTreeComponent()->StopTree();
}
