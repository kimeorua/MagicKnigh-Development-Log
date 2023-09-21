// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Weapon.h"
#include "ArmBarrier.h"
#include "Kismet/KismetMathLibrary.h"
#include "CollisionShape.h"
#include "Kismet/GameplayStatics.h"


APlayerCharacter::APlayerCharacter()
{
	// SpringArm 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // 
	CameraBoom->bUsePawnControlRotation = true; 

	//Camera 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 

	PlayerSetup();

	DefaultMappingContext = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	DashAction = nullptr;
	DodgeAction = nullptr;
	ComboAction = nullptr;
	Sword = nullptr;
	CurrentWeapon = nullptr;
	BlockAction = nullptr;
	bUseLockOn = false;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(LockOnEnemy)) //락온 판정 성공시, 카메라를 락온할 객체에 고정, 상하 시점 변경만 가능 하도록 구현
	{
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnEnemy->GetActorLocation());
		FRotator LockOnRotation = FRotator (GetController()->GetControlRotation().Pitch, LookAt.Yaw, LookAt.Roll);
		GetController()->SetControlRotation(LockOnRotation);
	}
}

void APlayerCharacter::PlayerSetup()
{
	// 플레이어 캡슐 컴포넌트 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 컨트롤러 회전시, 캐릭터 회전 안되도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 회전 설정
	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동방향으로 회전
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 회전 속도

	//캐릭터 움직인 관련 설정
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		ArmBarrier = GetWorld()->SpawnActor<AArmBarrier>(ArmBarrierClass);
		ArmBarrier->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ArmBarrier->GetEquipSocketName());

		Sword = GetWorld()->SpawnActor<AWeapon>(SwordClass);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::MoveEnd);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		//달리기
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &APlayerCharacter::DashEnd);

		//회피(구르기 & 스탭)
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);

		//무기 장착
		EnhancedInputComponent->BindAction(EquipActions[0], ETriggerEvent::Triggered, this, &APlayerCharacter::SwordSummons);

		//일반공격(콤보)
		EnhancedInputComponent->BindAction(ComboAction, ETriggerEvent::Started, this, &APlayerCharacter::ComboAttack);

		//스킬 사용
		EnhancedInputComponent->BindAction(QSkillAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UseQSkill); //Q Skill
		EnhancedInputComponent->BindAction(ESkillAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UseESkill); // E Skill
		EnhancedInputComponent->BindAction(EFSkillAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UseEFSkill); //특수 스킬

		//방어 사용
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &APlayerCharacter::BlockStart);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &APlayerCharacter::BlockEnd);

		//Lock On 사용
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &APlayerCharacter::LockOn);
	}
}
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.DoNotMove"))) <= 0)
		{
			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}

		//플레이어에게 Tag 부착, 현제 태그가 있의면 부착하지 않음.
		if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.Move"))) <= 0)
		{
			GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Move"))); //이동 중일때 부착
		}

		// 이동 방향에 따른 상태 태그 부착, -> 후에 타겟 락온 상태에서의 회피에 사용할 예정
		if (MovementVector.Y > 0)
		{
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveFwd"))) <= 0)
			{
				// 다른 방향 태그 삭제
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveBwd")));
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveLeft")));
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveRight")));

				//W 키를 눌름 -> 앞으로 이동 -> MoveFwd Tag 부착
				GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveFwd")));
			}
		}
		else if (MovementVector.Y < 0)
		{
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveBwd"))) <= 0)
			{
				// 다른 방향 태그 삭제
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveFwd")));
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveLeft")));
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveRight")));

				//S 키를 눌름 -> 뒤로 이동 -> MoveBwd Tag 부착
				GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveBwd")));
			}
		}
		else if (MovementVector.X < 0)
		{
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveLeft"))) <= 0)
			{
				// 다른 방향 태그 삭제
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveFwd")));
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveBwd")));
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveRight")));

				//A 키를 눌름 -> 좌로 이동 -> MoveLeft Tag 부착
				GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveLeft")));
			}
		}
		else if (MovementVector.X > 0)
		{
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveRight"))) <= 0)
			{
				// 다른 방향 태그 삭제
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveFwd")));
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveBwd")));
				GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveLeft")));

				//D 키를 눌름 -> 우로 이동 -> MoveRight Tag 부착
				GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveRight")));
			}
		}
	}
}

void APlayerCharacter::MoveEnd()
{
	//이동을 멈추면 이동 및 방향 Tag 제거
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Move")));
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveFwd")));
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveBwd")));
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveLeft")));
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveRight")));
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * LookRate);
		AddControllerPitchInput(LookAxisVector.Y * LookRate);
	}
}

// 달리기 사용 -> 이동 속도 증가 및 Dash Tag 부착
void APlayerCharacter::Dash()
{
	if (Controller != nullptr)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) <= 0)
			{
				GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
				if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.Dash"))) <= 0)
				{
					GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Dash")));
				}
			}
		}
	}
}

// 달리기 키에서 손을 뗌 -> Tag 제거 및 속도 원복
void APlayerCharacter::DashEnd()
{
	if (Controller != nullptr)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Dash")));
		}
	}
}

// 태긋 생성 및 해당 Tag를 가진 GamepalyAbility 작동
void APlayerCharacter::MakeTagAndActive(FString TagName)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName(TagName)));
	GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
}

//회피
void APlayerCharacter::Dodge()
{
	if (Controller != nullptr)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			if (bUseLockOn)
			{
				if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveFwd"))) > 0)
				{
					MakeTagAndActive("Player.Dodge.Rolling");
				}
				else if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveBwd"))) > 0)
				{
					MakeTagAndActive("Player.Dodge.Rolling.Bwd");
				}
				else if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveLeft"))) > 0)
				{
					MakeTagAndActive("Player.Dodge.Rolling.Left");
				}
				else if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.MoveRight"))) > 0)
				{
					MakeTagAndActive("Player.Dodge.Rolling.Right");
				}
			}
			else if (!bUseLockOn)
			{
				MakeTagAndActive("Player.Dodge.Rolling");
			}
		}
	}
}

//콤보 공격
void APlayerCharacter::ComboAttack()
{
	if (!(GetCharacterMovement()->IsFalling()))
	{
		MakeTagAndActive("Player.Attack");
	}
}

//스킬 사용(Q)
void APlayerCharacter::UseQSkill()
{
	if (!(GetCharacterMovement()->IsFalling()))
	{
		MakeTagAndActive("Player.Skill.QSkill");
	}
}

//스킬 사용(E)
void APlayerCharacter::UseESkill()
{
	if (!(GetCharacterMovement()->IsFalling()))
	{
		MakeTagAndActive("Player.Skill.ESkill");
	}
}

// 특수 스킬 사용
void APlayerCharacter::UseEFSkill()
{
	if (!(GetCharacterMovement()->IsFalling()))
	{
		MakeTagAndActive("Player.Skill.EFSkill");
	}
}

//방어 시작
void APlayerCharacter::BlockStart()
{
	if (!(GetCharacterMovement()->IsFalling()))
	{
		if (!CanUseParrying)
		{
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))) <= 0)
			{
				GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))); 
				CanUseParrying = true;

				FTimerHandle ParryingEndHandle;
				GetWorld()->GetTimerManager().SetTimer(ParryingEndHandle, FTimerDelegate::CreateLambda([&]()
					{
						GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying")));

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(ParryingEndHandle);
					}), InDelayTime, false); 
			}
		}
		if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) <= 0)
		{
			MakeTagAndActive("Player.Block");
		}
	}
}

// 방어 종료
void APlayerCharacter::BlockEnd()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) > 0)
	{
		CanUseParrying = false;
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock")));
		ArmBarrier->BarrierOff();
	}
}

//검 소환
void APlayerCharacter::SwordSummons()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Weapon.Sword"))) <= 0)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			WeaponUnequip();
			MakeTagAndActive("Player.EquipWeapon.Sword");
		}
	}
}

// 검을 플레이어 소켓에 장착
void APlayerCharacter::WeaponEquip(FName EquipSocketName, AWeapon* Weapon)
{
	if (Weapon != nullptr)
	{
		CurrentWeapon = Weapon;
		if (CurrentWeapon->GetSoummonsParticle() == nullptr) return;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentWeapon->GetSoummonsParticle(), CurrentWeapon->GetActorTransform());

		if (CurrentWeapon->GetSoummonSound() == nullptr) return;
		UGameplayStatics::PlaySound2D(this, CurrentWeapon->GetSoummonSound());

		Weapon->SetOwner(this);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipSocketName);
	}
}

// 이미 장착된 무기를 제거 후 안보이는 곳으로 이동
void APlayerCharacter::WeaponUnequip()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->SetOwner(CurrentWeapon);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		CurrentWeapon->SetActorLocation(FVector(0.f));
	}
}

void APlayerCharacter::TakeDamageFromEnemy()
{
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle;

	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) > 0)
	{
		if (GetInstigator())
		{
			float BlockAbleRot = 360.f - FMath::Abs(GetActorRotation().Yaw - GetInstigator()->GetActorRotation().Yaw);
			bool CheakBlock = UKismetMathLibrary::BooleanOR(BlockAbleRot > 150.f, BlockAbleRot < -150.f);
			AEnemyCharacter* AttackedEnemy = Cast<AEnemyCharacter>(GetInstigator());

			if (!(BlockAbleRot < 130.f || BlockAbleRot > 230.f))
			{
				if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))) > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Parrying"));
					SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[2], 1, EffectContext);
					AttackedEnemy->TakeParrying();
				}
				else if (AttackedEnemy->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.BreakBlock"))) > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Break Block")); 
					SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[0], 1, EffectContext);
				}
				else
				{
					SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[1], 1, EffectContext);
				}
				
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Hit"));
				SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[0], 1, EffectContext);
			}
		}
		
	}
	else
	{
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack.Combo1")));
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack.Combo2")));
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack.Combo3")));
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack.Combo4")));
		GetAbilitySystemComponent()->CancelAbilities();

		SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[0], 1, EffectContext);
	}

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void APlayerCharacter::EFCharge()
{
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle;
	SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[3], 1, EffectContext);

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void APlayerCharacter::LockOn()
{
	if (LockOnEnemy != nullptr) //락온 해제
	{
		LockOnEnemy = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		bUseLockOn = false;
	}
	else if (LockOnEnemy == nullptr) //락온 작동
	{
		FVector Start = GetActorLocation();
		FVector End = GetActorLocation() + (UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 500.f);
		TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());
		ActorsToIgnore.Add(ArmBarrier);
		ActorsToIgnore.Add(Sword);
		FHitResult OutHit;
		bool bResult;

		bResult = UKismetSystemLibrary::SphereTraceSingle
		(
			GetWorld(),
			Start,
			End,
			125.f,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel4),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true
		);

		if (bResult)
		{
			LockOnEnemy = Cast<AEnemyCharacter>(OutHit.GetActor());
			if (LockOnEnemy)
			{
				GetCharacterMovement()->bOrientRotationToMovement = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
				bUseLockOn = true;
			}
		}
	}
}