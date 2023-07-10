// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
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


APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	DefaultMappingContext = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	DashAction = nullptr;
	DodgeAction = nullptr;
	ComboAction = nullptr;
	Sword = nullptr;
	CurrentWeapon = nullptr;
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

		Sword = GetWorld()->SpawnActor<AWeapon>(SwordClass);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

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
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.Dash"))) <= 0)
			{
				GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Dash")));
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

void APlayerCharacter::Dodge()
{
	if (Controller != nullptr)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Dodge.Rolling")));
			GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
		}
	}
}

void APlayerCharacter::SwordSummons()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.Weapon.Sword"))) <= 0)
	{
		if (!(GetCharacterMovement()->IsFalling()))
		{
			WeaponUnequip();
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.EquipWeapon.Sword")));
			GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
		}
	}
}

void APlayerCharacter::WeaponEquip(FName EquipSocketName)
{
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipSocketName);
	CurrentWeapon = Sword;
}

void APlayerCharacter::WeaponUnequip()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		CurrentWeapon->SetActorLocation(FVector(0.f));
	}
}

void APlayerCharacter::ComboAttack()
{
	if (!(GetCharacterMovement()->IsFalling()))
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));
		GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
	}
}
