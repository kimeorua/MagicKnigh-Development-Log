// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicKnightAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

UMagicKnightAttributeSet::UMagicKnightAttributeSet()
{

}

void UMagicKnightAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth())); //HP�� ���Ѽ� �� ���Ѽ� ����
	}
	else if (Data.EvaluatedData.Attribute == GetElementalForceAttribute())
	{
		SetElementalForce(FMath::Clamp(GetElementalForce(), 0.f, GetMaxElementalForce())); //EF�� ���Ѽ� �� ���Ѽ� ����
	}
	else if (Data.EvaluatedData.Attribute == GetPostureAttribute())
	{
		SetPosture(FMath::Clamp(GetPosture(), 0.f, GetMaxPosture())); //ü���� ���Ѽ� �� ���Ѽ� ����
	}
	else if (Data.EvaluatedData.Attribute == GetHealingCountAttribute())
	{
		SetHealingCount(FMath::Clamp(GetHealingCount(), 0.f, GetMaxHealingCount())); //ȸ�� ��ų Ƚ���� ���Ѽ� �� ���Ѽ� ����
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute()) //�������� ���� ����� -> �������� ����
	{
		//HP ����
		SetHealth(FMath::Clamp(GetHealth() - Damage.GetCurrentValue(), 0.f, GetMaxHealth()));
		//���� ������ * 1.2�� ü�� ���
		SetPosture(FMath::Clamp(GetPosture() + Damage.GetCurrentValue() * 1.2f, 0.f, GetMaxPosture()));

		//HP�� 0���� -> ��� ����
		if (GetHealth() <= 0.f)
		{
			SetHealth(0.f); //HP�� 0���� ����
			ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor()); //�θ� Ŭ���� �����͸� �̿��Ͽ�, �� �� �÷��̾� ĳ������ Die(��� �Լ�)�� ȣ�� ��.
			Owner->Die();
		}

		else //HP�� ���� ������
		{
			if (GetPosture() >= GetMaxPosture()) //ü���� ���Ͽ�, ü�� �ִ�ġ �̻��̸� ���� ����
			{
				SetPosture(0.f); //ü�� �ʱ�ȭ
				ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor());//�θ� Ŭ���� �����͸� �̿��Ͽ�, �� �� �÷��̾� ĳ������ Stun(���� �Լ�)�� ȣ�� ��.
				//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Owner->GetName())
				Owner->Stun();
			}
		}

		Damage = 0.f; //������ �ʱ�ȭ 
	}

	else if (Data.EvaluatedData.Attribute == GetChargeEFAttribute()) //���� �� EF ����
	{
		SetElementalForce(FMath::Clamp(GetElementalForce() + ChargeEF.GetCurrentValue(), 0, GetMaxElementalForce())); //������ ���� �ִ�ġ�� ���� �ʵ���,
		ChargeEF = 0.f; //����ġ �ʱ�ȭ
	}

	else if (Data.EvaluatedData.Attribute == GetPostureUpAttribute()) //ü�� ���(ü�� ���Ҵ� ����) -> ���� ���� �� �۵�
	{
		SetPosture(FMath::Clamp(GetPosture() + PostureUp.GetCurrentValue(), 0, GetMaxPosture()));

		if (GetPosture() >= GetMaxPosture()) //ü���� ���Ͽ�, ü�� �ִ�ġ �̻��̸� ���� ����
		{
			SetPosture(0.f);
			ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor()); //�θ� Ŭ���� �����͸� �̿��Ͽ�, �� �� �÷��̾� ĳ������ Stun(���� �Լ�)�� ȣ�� ��.
			//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Owner->GetName())
			Owner->Stun();
		}
		PostureUp = 0.f; //ü�� ���ġ �ʱ�ȭ
	}

	else if (Data.EvaluatedData.Attribute == GetHealing_HPAttribute()) //ü�� ȸ�� ��
	{
		if (GetHealth() < GetMaxHealth() && GetHealingCount() > 0) //�ִ� ü���϶�, ���Ƚ���� 0���� �������� ��� �Ұ�
		{
			SetHealingCount(GetHealingCount() - 1); //Ƚ�� ����
			SetHealth(FMath::Clamp(GetHealth(), GetHealth() + Healing_HP.GetCurrentValue(), GetMaxHealth())); //HP ���
			//UE_LOG(LogTemp, Warning, TEXT("Heal")); 
		}
		SetHealing_HP(0.f); //ȸ���� �ʱ�ȭ
	}
}

void UMagicKnightAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, Health, COND_None,  REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, ElementalForce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxElementalForce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, Posture, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxPosture, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, ChargeEF, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, HealingCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMagicKnightAttributeSet, MaxHealingCount, COND_None, REPNOTIFY_Always);
}

void UMagicKnightAttributeSet::OnRep_Health(FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Health, OldHealth);
}

void UMagicKnightAttributeSet::OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxHealth, OldMaxHealth);
}

void UMagicKnightAttributeSet::OnRep_Posture(FGameplayAttributeData& OldPosture)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Posture, OldPosture);
}

void UMagicKnightAttributeSet::OnRep_MaxPosture(FGameplayAttributeData& OldMaxPosture)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxPosture, OldMaxPosture);
}

void UMagicKnightAttributeSet::OnRep_ElementalForce(FGameplayAttributeData& OldElementalForce)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, ElementalForce, OldElementalForce);
}

void UMagicKnightAttributeSet::OnRep_MaxElementalForce(FGameplayAttributeData& OldMaxElementalForce)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxElementalForce, OldMaxElementalForce);
}

void UMagicKnightAttributeSet::OnRep_Damage(FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Damage, OldDamage);
}

void UMagicKnightAttributeSet::OnRep_ChargeEF(FGameplayAttributeData& OldChargeEF)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, ChargeEF, OldChargeEF);
}

void UMagicKnightAttributeSet::OnRep_PostureUp(FGameplayAttributeData& OldPostureUp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, PostureUp, OldPostureUp);
}

void UMagicKnightAttributeSet::OnRep_HealingCount(FGameplayAttributeData& OldHealingCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, HealingCount, OldHealingCount);
}

void UMagicKnightAttributeSet::OnRep_MaxHealingCount(FGameplayAttributeData& OldMaxHealingCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, MaxHealingCount, OldMaxHealingCount);
}

void UMagicKnightAttributeSet::OnRep_OldHealing_HP(FGameplayAttributeData& OldHealing_HP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMagicKnightAttributeSet, Healing_HP, OldHealing_HP);
}
