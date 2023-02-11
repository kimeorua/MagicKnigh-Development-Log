// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainAnimInstance.h"
#include "Shield.h"
#include "Weapon.h"

AMainCharacter::AMainCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 

	CurrentSpeed = ForwardWalkSpeed;
	bUseDash = false;
	State = MoveState::MS_Move;
	MainAnimInstance = nullptr;
	IsCombo = false;
	CurrentCombo = 1;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentSpeed = ForwardWalkSpeed;
	bUseDash = false;

	State = MoveState::MS_Move;
	MainAnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());

	Shield = GetWorld()->SpawnActor<AShield>(ShieldClass);
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ShieldSocket"));
	Shield->SetOwner(this);

	Sword = GetWorld()->SpawnActor<AWeapon>(SwordClass);
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponEquipSocket"));
	Sword->SetOwner(this);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("Look Up", this, &AMainCharacter::LookRight);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AMainCharacter::DashEnd);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AMainCharacter::Dodge);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMainCharacter::BlockStart);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AMainCharacter::BlockEnd);

	PlayerInputComponent->BindAction("Sword Equip", IE_Pressed, this, &AMainCharacter::SelectSword);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainCharacter::LMBDawn);

	PlayerInputComponent->BindAction("QSkill", IE_Pressed, this, &AMainCharacter::QSkillActivated);
	PlayerInputComponent->BindAction("ESkill", IE_Pressed, this, &AMainCharacter::ESkillActivated);
}

void AMainCharacter::MoveForward(float Value)
{
	if (State != MoveState::MS_Equip && State != MoveState::MS_Attack && State != MoveState::MS_Skill)
	{
		if ((Controller != nullptr) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);

			if (Value > 0)
			{
				GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
			}
			else if (Value < 0)
			{
				GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed - BackwardSpeed;
			}

		}
		if (Value >= 0)
		{
			MoveNum = 1;
		}
		else if (Value < 0)
		{
			MoveNum = 2;
		}
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (State != MoveState::MS_Equip && State != MoveState::MS_Attack)
	{
		if ((Controller != nullptr) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
			GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
		}

		if (Value > 0)
		{
			MoveNum = 3;
		}
		else if (Value < 0)
		{
			MoveNum = 4;
		}
	}
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerYawInput(Value * 0.5);
}

void AMainCharacter::LookRight(float Value)
{
	AddControllerPitchInput(Value * 0.5);
}

void AMainCharacter::Dash()
{
	if (State == MoveState::MS_Move)
	{
		if (!bUseDash)
		{
			bUseDash = true;
			CurrentSpeed = ForwardRunSpeed;
		}
	}	
}

void AMainCharacter::DashEnd()
{
	if (bUseDash)
	{
		bUseDash = false;
		CurrentSpeed = ForwardWalkSpeed;
	}
}

void AMainCharacter::Dodge()
{
	if (State != MoveState::MS_Equip && State != MoveState::MS_Attack)
	{
		State = MoveState::MS_Dodge;
		MainAnimInstance->PlayDodge(MoveNum);
		bUseControllerRotationYaw = false;
		bUseBlock = false;
	}
	else
	{
		return;
	}
}

void AMainCharacter::BlockStart()
{
	if (State == MoveState::MS_Move && !bUseDash && State != MoveState::MS_Attack)
	{
		bUseBlock = true;
		State = MoveState::MS_Block;
	}
	else
	{
		return;
	}
}

void AMainCharacter::BlockEnd()
{
	if (State == MoveState::MS_Block && !bUseDash && State != MoveState::MS_Attack)
	{
		bUseBlock = false;
		State = MoveState::MS_Move;
	}
	else
	{
		return;
	}
}

void AMainCharacter::SelectSword()
{
	if (State == MoveState::MS_Move)
	{
		State = MoveState::MS_Equip;
		WeaponNum = 1;
		WeaponEquip();
	}
	else { return; }
}

void AMainCharacter::WeaponEquip()
{
	if (UseWeaponNum == WeaponNum) 
	{ 
		State = MoveState::MS_Move;
		return; 
	}
	else
	{
		if (WeaponNum == 1)
		{
			Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponHandSocket"));
			UseWeaponNum = 1;
			CurrentWeapon = Sword;
			MainAnimInstance->PlayEquip();
		}
	}
}

void AMainCharacter::LMBDawn()
{
	if (CurrentWeapon != nullptr)
	{
		if (State != MoveState::MS_Attack && State == MoveState::MS_Move)
		{
			Attack();
		}
		else if (State == MoveState::MS_Attack)
		{
			IsCombo = true;
		}
	}
	else { return; }
}

void AMainCharacter::Attack()
{
	State = MoveState::MS_Attack;
	if (MainAnimInstance == nullptr) { return; }

	SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
	MainAnimInstance->PlayAttack(CurrentCombo);
	//UE_LOG(LogTemp, Warning, TEXT("Combo: %d"), CurrentCombo);
}

void AMainCharacter::QSkillActivated()
{
	if (CurrentWeapon != nullptr)
	{
		if (State == MoveState::MS_Move)
		{
			State = MoveState::MS_Skill;
			MainAnimInstance->PlaySkill('Q');
		}
	}
}

void AMainCharacter::ESkillActivated()
{
	if (CurrentWeapon != nullptr)
	{
		if (State == MoveState::MS_Move)
		{
			State = MoveState::MS_Skill;
			MainAnimInstance->PlaySkill('E');
		}
	}
}

void AMainCharacter::AttackEnd()
{
	State = MoveState::MS_Move;
	IsCombo = false;
	CurrentCombo = 1;
}

void AMainCharacter::DodgeEnd()
{
	if (State != MoveState::MS_Equip && State != MoveState::MS_Attack)
	{
		State = MoveState::MS_Move;
		bUseControllerRotationYaw = true;
	}
	else
	{
		return;
	}
}

void AMainCharacter::CheackCombo()
{
	if (CurrentCombo >= MaxCombo) 
	{
		CurrentCombo = 0; 
	}
	if (IsCombo == true) 
	{
		CurrentCombo += 1; 
		IsCombo = false; 
		Attack();
	}
}
