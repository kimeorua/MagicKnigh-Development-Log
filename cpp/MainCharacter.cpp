// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainAnimInstance.h"

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
	State = MainState::MS_Move;
	MainAnimInstance = nullptr;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentSpeed = ForwardWalkSpeed;
	bUseDash = false;

	State = MainState::MS_Move;
	MainAnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
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
}

void AMainCharacter::MoveForward(float Value)
{
	if (State == MainState::MS_Move)
	{
		if (Value > 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
		}
		else if (Value < 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed - BackwardSpeed;
		}

		if ((Controller != nullptr) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
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

void AMainCharacter::MoveRight(float Value)
{
	if (State == MainState::MS_Move)
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;

		if ((Controller != nullptr) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
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
	if (State == MainState::MS_Move)
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
	State = MainState::MS_Dodge;
	MainAnimInstance->PlayDodge(MoveNum);
}

void AMainCharacter::DodgeEnd()
{
	State = MainState::MS_Move;
}
