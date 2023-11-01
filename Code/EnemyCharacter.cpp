// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.h"
#include "EnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "CombetComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	AIController = nullptr;

	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);
	HitCollision->SetCollisionProfileName(TEXT("EnemyHit"));
	
	UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI"));
	UI->SetupAttachment(RootComponent);

	HitSound = nullptr;
	HitParticle = nullptr;
	EnemyAnim = nullptr;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AEnemyAIController>(GetController());
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	EnemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance()); //�ִϸ��̼� ȣ���� ���� ����
	if (IsValid(EnemyAnim)) { EnemyAnim->OnMontageEnded.AddDynamic(this, &AEnemyCharacter::OnAttackMontageEnded); } //��Ÿ�� ����� OnAttackMontageEnded()�Լ� ȣ��
}

//�÷��̾� ã��
void AEnemyCharacter::FindPlayer()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed; //�̵� �ӵ� ����
}

//�÷��̾� ��ħ
void AEnemyCharacter::LosePlayer()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; //�̵� �ӵ� ����
}

//�÷��̾�� ������ ����
void AEnemyCharacter::TakeDamgeFormPlayer(EDamageEffectType DamageType)
{
	if (!bIsDie) //���� �������°� �ƴ��� �Ǵ�
	{
		GetWorldTimerManager().PauseTimer(PostureHandle);
		//UE_LOG(LogTemp, Warning, TEXT("Hit"));
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext(); //EffectContext ����
		EffectContext.AddSourceObject(this); //SourceObject�� �� ĳ���� �ڽ� �߰�
		FGameplayEffectSpecHandle SpecHandle;

		if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.SuperArmor"))) > 0) //���� ���� �Ƹ� ���� �̸� �ǰ� ��� x ����Ʈ �� ���常 ���
		{
			if (IsValid(HitSound) && HitParticle != nullptr)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetMesh()->GetSocketLocation(HitParticleSocket));
			}
		}
		//DamageEffect�� SpecHandle�� ������.
		SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffects[DamageType], 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			//SpecHandle�� ���� GamepalyEffect �۵�
			FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
//�浹 ó�� �Լ�
// "ECollisionChannel::ECC_GameTraceChannel3" �� �÷��̾� ĸ�� ������Ʈ�� ����� Traceä�� -> �浹�� �÷��̾� ĳ���Ϳ��� ������
FHitResult AEnemyCharacter::CheakCollision(EAttackCollisionType Type, float Range, EDamageEffectType DamageType, EAttackDirectionType DriectionType)
{
	if (Type == EAttackCollisionType::Range_Line)
	{
		APlayerCharacter* Target = Cast<APlayerCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
		if (IsValid(Target))
		{
			GetCombetComponent()->SetTargetLocation(Target->GetActorLocation());
		}
	}
	TTuple<bool, FHitResult> CollisionResult = GetCombetComponent()->AttackCollision(Type, Range, DriectionType, ECollisionChannel::ECC_GameTraceChannel3);
	bool isHit = CollisionResult.Get<0>();
	FHitResult OutHit = CollisionResult.Get<1>();

	if (isHit) //�浹 �ϸ�
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OutHit.GetActor()); //�浹�� ��ü�� Player�� ����ȯ 
		if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0) //�÷��̾� ĳ���Ͱ� ��� ���� �ʾ����� ������ �ֱ� 
		{
			if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseDodge"))) <= 0) 
			{
				if (Player && !PlayerIsHit) 
				{
					Player->SetInstigator(this); //�����ڸ� �ش� ��ü�� ����  
					Player->TakeDamageFromEnemy(DamageType); //������ ���� Ÿ�Կ� �´� ������ ����Ʈ �۵� 
					PlayerIsHit = true; //�÷��̾ ���ݿ� ������ -> �� ���ݿ� 1���� �ǰ� ���� ���� 
				}
			}
		}
		else if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) > 0)//ĳ���Ͱ� ����� �����̸� 
		{
			// �����忡 ������ �÷��̾� ��ü�� �ʱ�ȭ �ϰ� �÷��̾� �߰� ���θ� false�� ���� 
			AIController->GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::Player, nullptr);
			AIController->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::CanSeePlayer, false);
			// �÷��̾� �ٶ��� �ʰ�, ��Ʈ�� �ǽ���.
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
	return OutHit; //Hit��� ����ü ��ȯ
}

//�� ĳ���� ���
void AEnemyCharacter::Die()
{
	//�θ� Ŭ������ ����Լ� ȣ�� -> ������θ� true�� ����
	Super::Die();
	// �� ��ü�� AI �����忡 ��������� ���� -> �����̺�� Ʈ�� �۵� ����
	AIController->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsDie, true);
	//��ĳ���Ͱ� ��� ��������, �÷��̾��� LockOn�� ������
	APlayerCharacter* Player = Cast<APlayerCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	Player->LockOnReset();
	//��Ŀ�� ����
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
}

//�������Ʈ���� ���� ������, �ٽ� �����ϱ� ���� �ʱ�ȭ �ϴ� �Լ�
void AEnemyCharacter::PlayerHitReset()
{
	PlayerIsHit = false;
}

//�÷��̾ �� ĳ������ ������ ƨ�ܳ� -> ü�� ���
void AEnemyCharacter::TakeParrying()
{
	//���� ���°� �ƴϸ�
	if (!bIsDie)
	{
		GetWorldTimerManager().PauseTimer(PostureHandle);
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext(); 
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle SpecHandle;

		if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.Parryable"))) > 0) //���� ����� ���������� ���� �ٸ��� ü���� ��� ��Ŵ
		{
			SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffects[EDamageEffectType::PostureUp_OnParry], 1, EffectContext); 
			Super::ReStartPostureTimer();
		}
		else
		{
			SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffects[EDamageEffectType::PostureUp], 1, EffectContext);
		}

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AEnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast(); //BTTask_EnemyAttack���� ������ ���� ���� �Լ��� ȣ��ǵ��� ��������Ʈ�� �̿��Ͽ� ����
}
