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
#include "Blueprint/UserWidget.h"


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
	
	//카메라 및 이동 기본 세팅
	PlayerSetup();

	//초기화
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
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnEnemy->GetActorLocation()); //바라볼 방향 
		FRotator LockOnRotation = FRotator (GetController()->GetControlRotation().Pitch, LookAt.Yaw, LookAt.Roll); //방향으로 회전할 회전 값
		GetController()->SetControlRotation(LockOnRotation); //캐릭터 회전
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

		//힐링 사용
		EnhancedInputComponent->BindAction(HealingAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Healing);

		//게임 일시 정지
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::GamePause);
	}
}
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
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
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
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
}

// 달리기 사용 -> 이동 속도 증가 및 Dash Tag 부착
void APlayerCharacter::Dash()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
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
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0) //사망중이면 사용 불가
	{
		if (Controller != nullptr)
		{
			if (!(GetCharacterMovement()->IsFalling()))
			{
				if (bUseLockOn) //LockOn 상태이면 방향에 따르게 다른 회피 작동
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
				else if (!bUseLockOn) //LockOn 중이 아니면 일반 회피 작동
				{
					MakeTagAndActive("Player.Dodge.Rolling");
				}
			}
		}
	}
}

//콤보 공격
void APlayerCharacter::ComboAttack()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			MakeTagAndActive("Player.Attack");
		}
	}
}

//스킬 사용(Q)
void APlayerCharacter::UseQSkill()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			MakeTagAndActive("Player.Skill.QSkill");
		}
	}
}

//스킬 사용(E)
void APlayerCharacter::UseESkill()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			MakeTagAndActive("Player.Skill.ESkill");
		}
	}
}

// 특수 스킬 사용
void APlayerCharacter::UseEFSkill()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
	{ 
		if (!(GetCharacterMovement()->IsFalling()))
		{
			MakeTagAndActive("Player.Skill.EFSkill");
		}
	}
}

//방어 시작
void APlayerCharacter::BlockStart()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			if (!UseParrying) //튕겨내기 사용 중이 아니면 -> 키를 처음 눌름
			{
				if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))) <= 0) //1번만 부착하기 위해 Tag 검사
				{
					GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))); // Tag 부착
					UseParrying = true; //튕겨내기 사용 중으로 변경 -> 가드 작동시, 일정 시간에만 튕겨내기 작동함.

					//Timer 설정
					ParryingEndHandle; 
					GetWorld()->GetTimerManager().SetTimer(ParryingEndHandle, FTimerDelegate::CreateLambda([&]()
						{
							GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))); //InDelayTime이 지난후 Tag 제거

							// 타이머 초기화
							GetWorld()->GetTimerManager().ClearTimer(ParryingEndHandle);
						}), InDelayTime_Parry, false);
				}
			}
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) <= 0) // 방어 사용 중이 아니면  작동
			{
				MakeTagAndActive("Player.Block"); 
			}
		}
	}
}

// 방어 종료
void APlayerCharacter::BlockEnd()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) > 0)
	{
		UseParrying = false; // 가드 종료 -> 튕겨내기 다시 사용 할 수 있도록 변경
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))); //가드 사용중 Tag 제거
		ArmBarrier->BarrierOff(); //방패 크기 초기화
	}

	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))) > 0)
	{
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))); //가드 사용중 Tag 제거
	}
}

//검 소환
void APlayerCharacter::SwordSummons()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
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

//적에게 공격을 받을 시.
void APlayerCharacter::TakeDamageFromEnemy(EDamageEffectType DamageType)
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0) //플레이어가 사망 하지 않았을때 작동
	{
		GetWorldTimerManager().PauseTimer(PostureHandle);

		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle SpecHandle;

		if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) > 0) //현재 방어 중이면
		{
			if (GetInstigator())
			{
				float BlockAbleRot = 360.f - FMath::Abs(GetActorRotation().Yaw - GetInstigator()->GetActorRotation().Yaw); //적 캐릭터의 방향과 플레이어 캐릭터의 방향 각도를 계산
				AEnemyCharacter* AttackedEnemy = Cast<AEnemyCharacter>(GetInstigator()); //공격한 객체 저장

				if (!(BlockAbleRot < 130.f || BlockAbleRot > 230.f)) //방어각도(100')에 들어 왔으면 방어 or 튕겨내기 성공
				{
					if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseParrying"))) > 0) //튕겨내기 Effect
					{
						//UE_LOG(LogTemp, Warning, TEXT("Parrying"));
						SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(CombetEffects[ECombetEffectType::Parrying], 1, EffectContext);
						AttackedEnemy->TakeParrying();
						EFCharge();
					}
					else
					{
						if (AttackedEnemy->GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Enemy.State.BreakBlock"))) > 0) //적 캐릭터가 방어 무시 공격을 하면
						{
							//UE_LOG(LogTemp, Warning, TEXT("Break Block"));
							SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffects[DamageType], 1, EffectContext); //데미지 받음
						}
						else //방어 성공시
						{
							SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(CombetEffects[ECombetEffectType::Block], 1, EffectContext); //방어 Effect
						}
					}
				}
				else
				{
					SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffects[DamageType], 1, EffectContext); //데미지 받음
				}
			}

		}

		//방어 중이 아닐때 
		else
		{
			//일반 공격 중이면 테그 제거
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack.Combo1")));
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack.Combo2")));
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack.Combo3")));
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack.Combo4")));
			//어빌리티 사용 취소
			GetAbilitySystemComponent()->CancelAbilities();

			SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffects[DamageType], 1, EffectContext); //데미지 받음
		}

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()); //이펙트가 실제로 적용됨
		}
	}
}

//EF 충전
void APlayerCharacter::EFCharge()
{
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle;
	SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(CombetEffects[ECombetEffectType::Charge], 1, EffectContext);

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

//LockOn 초기화
void APlayerCharacter::LockOnReset()
{
	LockOnEnemy = nullptr;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseLockOn = false;
}

//플레이어 사망
void APlayerCharacter::Die()
{
	Super::Die();
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Die")));
	}
	//충돌 방지를 위하여, Tag 제거
	DashEnd();
	MoveEnd();
	BlockEnd();
	//플레이어 사망시, LockOn 해제
	LockOnReset();

	GetWorld()->GetTimerManager().ClearTimer(ParryingEndHandle);
}

void APlayerCharacter::LockOn()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
	{
		if (LockOnEnemy != nullptr) //락온 해제
		{
			LockOnReset();
		}
		else if (LockOnEnemy == nullptr) //락온 작동
		{
			FVector Start = GetActorLocation(); //현재 플레이어의 위치
			FVector End = GetActorLocation() + (UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 1200.f); //락온 범위
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
				EDrawDebugTrace::None,
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
}

//회복 스킬 사용
void APlayerCharacter::Healing()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Die"))) <= 0)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseHealing"))) <= 0)
			{
				MakeTagAndActive("Player.Healing");
			}
		}
	}
}

void APlayerCharacter::GamePause()
{
	PauseWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PauseWidgetClass);

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	PauseWidget->AddToViewport();
}
