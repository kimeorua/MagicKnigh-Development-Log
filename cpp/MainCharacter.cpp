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
	//ĸ�� ������Ʈ ũ��
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//ī�޶� �ʱ�ȭ
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 
	//-----------------------------------------------------------------------------------------------------//

	//���� �ӵ�, �޸��� ��� ����, �ִ��ν��Ͻ� �ʱ�ȭ
	CurrentSpeed = ForwardWalkSpeed;
	bUseDash = false;
	MainAnimInstance = nullptr;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	// ���� ���۽� �����ӵ��� �ȱ� �ӵ��� ����
	CurrentSpeed = ForwardWalkSpeed;
	bUseDash = false;

	// �ִ��ν��Ͻ� ����
	MainAnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());

	//���� ���� �� ����
	Shield = GetWorld()->SpawnActor<AShield>(ShieldClass);
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ShieldSocket"));
	Shield->SetOwner(this);

	// �� ���� �� ����
	Sword = GetWorld()->SpawnActor<AWeapon>(SwordClass);
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponEquipSocket"));
	Sword->SetOwner(this); 
	MaxCombo = 0;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�� �Է�
	PlayerInputComponent->BindAxis("Move Forward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("Look Up", this, &AMainCharacter::LookRight);
	//-----------------------------------------------------------------------------------------------------//
	
	// �׼� �Է�
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AMainCharacter::DashEnd);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AMainCharacter::Dodge);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainCharacter::LMBDawn);
	//-----------------------------------------------------------------------------------------------------//
}

void AMainCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !IsAttack)
	{
		// �� ���� ���� ã��
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Forward vector ���
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value); // �ش� �������� �̵�

		//��, �ڿ� ���� �ӵ� ����
		if (Value > 0.f)
		{
			GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
		}
		else if (Value < 0.f)
		{
			GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed - BackwardSpeed;
		}
	}
	// ȸ�� ���� ����
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
	if ((Controller != nullptr) && (Value != 0.0f) && !IsAttack)
	{
		// ������ ���� ã��
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Right vector ��� 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
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
	// ���� �����Ƽ ��� �� �ƴϰ�
	if (!bUseAbility)
	{
		//�޸��� ���µ� �ƴҶ� �۵�
		if (!bUseDash)
		{
			// �޸��� ���·� ���� ��, �����Ƽ ��� �Ұ�
			bUseDash = true;
			bCanUseAbility = false;
			CurrentSpeed = ForwardRunSpeed;
		}
	}
}

void AMainCharacter::DashEnd()
{
	if (!bUseAbility)
	{
		if (bUseDash)
		{
			//�޸��� ���� ���� �� �����Ƽ ��� ����
			bUseDash = false;
			bCanUseAbility = true;
			CurrentSpeed = ForwardWalkSpeed;
		}
	}
}

void AMainCharacter::Dodge()
{
	// ���Ƽ ��� ���� �ƴҶ� �۵�
	if (!bUseAbility && !IsAttack)
	{
		// ȸ�� �ִϸ��̼� �۵� �� �����Ƽ ��� �Ұ��� ���� -> AnimNotify���� ���� �� �����Ƽ ��� �������� ���� ��
		MainAnimInstance->PlayDodge(MoveNum);
		bCanUseAbility = false;
		IsDodge = true;
	}
}

bool AMainCharacter::Equip(AWeapon* UseWeapon, FName EquipSocket, int32 EquipNum)
{
	//���� �� ����(UseWeapon), ������ ����(EquipSocket), ������ȣ(EquipNum)
	//�����Ƽ���� ������ �޾� �ش� ���Ⱑ �̹� ������ ����� �ٸ��� ���⿡ ������ ���Ͽ� ���� �� ���� ��ȣ ����
	//�ִ� �޺� Ƚ���� �� �ñ⿡ ����
	if (CurrentWeapon == UseWeapon)
	{
		return false;
	}
	else
	{
		CurrentWeapon = UseWeapon;
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EquipSocket);
		CurrentWeaponNum = EquipNum;
		MaxCombo = UseWeapon->GetAttackMaxCombo();
		return true;
	}
}

void AMainCharacter::LMBDawn()
{
	if (!bUseAbility && !IsDodge) //ȸ�� ���� �ƴϰ�, �����Ƽ ��� ���� �ƴҶ� �۵�
	{
		if (CurrentWeapon != nullptr) //���⸦ ���� �ؾ��� �۵�
		{
			bCanUseAbility = false; //�����Ƽ ��� �Ұ��� ����
			if (!(MainAnimInstance->Montage_IsPlaying(CurrentWeapon->GetAttackMontage()))) // ��Ÿ�ְ� �������� �ƴϸ� ó�� ����, �ƴϸ� �޺� �������� �Ǵ�
			{
				Attack();
				IsAttack = true;
			}
			else
			{
				IsCombo = true; //�޺� ������ ����
			}
		}
	}
}

void AMainCharacter::Attack()
{
	if (MainAnimInstance == nullptr) { return; }
	MainAnimInstance->PlayAttack(CurrentCombo);
	//UE_LOG(LogTemp, Warning, TEXT("Combo: %d"), CurrentCombo);
}

// ���� ���� �Լ� -> �޺� ����, ���� ���θ� false�� ����, ���� �޺� �ʱ�ȭ �� �����Ƽ ��� ���� ���·� ���� -> ��Ƽ���̸� ���� ȣ�� 
void AMainCharacter::AttackEnd()
{
	IsCombo = false;
	CurrentCombo = 1;
	IsAttack = false;
	bCanUseAbility = true;
}

// �޺� üũ�� �Լ� -> ��Ƽ���̸� ���� ȣ��
void AMainCharacter::CheackCombo()
{
	if (CurrentCombo >= MaxCombo)
	{
		CurrentCombo = 1;
	}
	if (IsCombo == true)
	{
		CurrentCombo += 1;
		IsCombo = false;
		Attack();
	}
}