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
	EnemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance()); //애니메이션 호출을 위한 변수
	if (IsValid(EnemyAnim)) { EnemyAnim->OnMontageEnded.AddDynamic(this, &AEnemyCharacter::OnAttackMontageEnded); } //몽타주 종료시 OnAttackMontageEnded()함수 호출
}

//플레이어 찾음
void AEnemyCharacter::FindPlayer()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed; //이동 속도 증가
}

//플레이어 놓침
void AEnemyCharacter::LosePlayer()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; //이동 속도 감소
}

//플레이어에게 데미지 받음
void AEnemyCharacter::TakeDamgeFormPlayer(EDamageEffectType DamageType)
{
	if (!bIsDie) //현재 죽은상태가 아닌지 판단
	{
		GetWorldTimerManager().PauseTimer(PostureHandle);
		//UE_LOG(LogTemp, Warning, TEXT("Hit"));
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext(); //EffectContext 생성
		EffectContext.AddSourceObject(this); //SourceObject에 적 캐릭터 자신 추가
		FGameplayEffectSpecHandle SpecHandle;

		if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.SuperArmor"))) > 0) //현재 슈퍼 아머 상태 이면 피격 모션 x 이펙트 및 사운드만 출력
		{
			if (IsValid(HitSound) && HitParticle != nullptr)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetMesh()->GetSocketLocation(HitParticleSocket));
			}
		}
		//DamageEffect를 SpecHandle에 저장함.
		SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffects[DamageType], 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			//SpecHandle을 통해 GamepalyEffect 작동
			FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
//충돌 처리 함수
// "ECollisionChannel::ECC_GameTraceChannel3" 은 플레이어 캡슐 컴포넌트에 적용된 Trace채널 -> 충돌시 플레이어 캐릭터에만 반응함
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

	if (isHit) //충돌 하면
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OutHit.GetActor()); //충돌한 객체를 Player로 형변환 
		if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0) //플레이어 캐릭터가 사망 하지 않았으면 데미지 주기 
		{
			if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseDodge"))) <= 0) 
			{
				if (Player && !PlayerIsHit) 
				{
					Player->SetInstigator(this); //유발자를 해당 객체로 변경  
					Player->TakeDamageFromEnemy(DamageType); //몬스터의 공격 타입에 맞는 데미지 이펙트 작동 
					PlayerIsHit = true; //플레이어가 공격에 적중함 -> 한 공격에 1번만 피격 판정 유도 
				}
			}
		}
		else if (Player->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) > 0)//캐릭터가 사망한 상태이면 
		{
			// 블랙보드에 기입한 플레이어 객체를 초기화 하고 플레이어 발견 여부를 false로 변경 
			AIController->GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::Player, nullptr);
			AIController->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::CanSeePlayer, false);
			// 플레이어 바라보지 않고, 패트롤 실시함.
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
	return OutHit; //Hit결과 구조체 반환
}

//적 캐릭터 사망
void AEnemyCharacter::Die()
{
	//부모 클래스의 사망함수 호출 -> 사망여부를 true로 변경
	Super::Die();
	// 이 객체의 AI 블랙보드에 사망했음를 적용 -> 비헤이비어 트리 작동 중지
	AIController->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsDie, true);
	//적캐릭터가 사망 했음으로, 플레이어의 LockOn을 해제함
	APlayerCharacter* Player = Cast<APlayerCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	Player->LockOnReset();
	//포커스 해제
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
}

//블루프린트에서 공격 종료후, 다시 판정하기 위해 초기화 하는 함수
void AEnemyCharacter::PlayerHitReset()
{
	PlayerIsHit = false;
}

//플레이어가 적 캐릭터의 공격을 튕겨냄 -> 체간 상승
void AEnemyCharacter::TakeParrying()
{
	//죽은 상태가 아니면
	if (!bIsDie)
	{
		GetWorldTimerManager().PauseTimer(PostureHandle);
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext(); 
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle SpecHandle;

		if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.Parryable"))) > 0) //경직 모션이 나오는지에 따라 다르게 체간을 상승 시킴
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
	OnAttackEnd.Broadcast(); //BTTask_EnemyAttack에서 정의한 공격 종료 함수가 호출되도록 델리게이트를 이용하여 구축
}
