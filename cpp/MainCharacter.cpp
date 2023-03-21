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
	//캡슐 컴포넌트 크기
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//카메라 초기화
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 
	//-----------------------------------------------------------------------------------------------------//

	//현제 속도, 달리기 사용 여부, 애님인스턴스 초기화
	CurrentSpeed = ForwardWalkSpeed;
	bUseDash = false;
	MainAnimInstance = nullptr;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 게임 시작시 현제속도를 걷기 속도로 변경
	CurrentSpeed = ForwardWalkSpeed;
	bUseDash = false;

	// 애님인스턴스 할장
	MainAnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());

	//방패 생성 밑 부착
	Shield = GetWorld()->SpawnActor<AShield>(ShieldClass);
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ShieldSocket"));
	Shield->SetOwner(this);

	// 검 생성 및 부착
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

	//축 입력
	PlayerInputComponent->BindAxis("Move Forward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("Look Up", this, &AMainCharacter::LookRight);
	//-----------------------------------------------------------------------------------------------------//
	
	// 액션 입력
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
		// 앞 진행 방향 찾기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Forward vector 계산
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value); // 해당 방향으로 이동

		//앞, 뒤에 따라 속도 변경
		if (Value > 0.f)
		{
			GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
		}
		else if (Value < 0.f)
		{
			GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed - BackwardSpeed;
		}
	}
	// 회피 방향 설정
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
		// 오른쪽 방향 찾기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Right vector 계산 
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
	// 현제 어빌리티 사용 이 아니고
	if (!bUseAbility)
	{
		//달리기 상태도 아닐때 작동
		if (!bUseDash)
		{
			// 달리기 상태로 변경 및, 어빌리티 사용 불가
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
			//달리기 상태 해제 및 어빌리티 사용 가능
			bUseDash = false;
			bCanUseAbility = true;
			CurrentSpeed = ForwardWalkSpeed;
		}
	}
}

void AMainCharacter::Dodge()
{
	// 어비리티 사용 중이 아닐때 작동
	if (!bUseAbility && !IsAttack)
	{
		// 회피 애니메이션 작동 및 어빌리티 사용 불가로 변경 -> AnimNotify에서 종료 시 어빌리티 사용 가능으로 변경 함
		MainAnimInstance->PlayDodge(MoveNum);
		bCanUseAbility = false;
		IsDodge = true;
	}
}

bool AMainCharacter::Equip(AWeapon* UseWeapon, FName EquipSocket, int32 EquipNum)
{
	//장착 할 무기(UseWeapon), 장착할 소켓(EquipSocket), 고유번호(EquipNum)
	//어빌리티에서 변수를 받아 해당 무기가 이미 장착한 무기와 다르면 무기에 지정된 소켓에 장착 및 고유 번호 저장
	//최대 콤보 횟수도 이 시기에 지정
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
	if (!bUseAbility && !IsDodge) //회피 중이 아니고, 어빌리티 사용 중이 아닐때 작동
	{
		if (CurrentWeapon != nullptr) //무기를 장착 해야지 작동
		{
			bCanUseAbility = false; //어빌리티 사용 불가로 변경
			if (!(MainAnimInstance->Montage_IsPlaying(CurrentWeapon->GetAttackMontage()))) // 몽타주가 실행중이 아니면 처음 공격, 아니면 콤보 공격으로 판단
			{
				Attack();
				IsAttack = true;
			}
			else
			{
				IsCombo = true; //콤보 중으로 변경
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

// 공격 종료 함수 -> 콤보 여부, 공격 여부를 false로 변경, 현제 콤보 초기화 및 어빌리티 사용 가능 상태로 변경 -> 노티파이를 통해 호출 
void AMainCharacter::AttackEnd()
{
	IsCombo = false;
	CurrentCombo = 1;
	IsAttack = false;
	bCanUseAbility = true;
}

// 콤보 체크용 함수 -> 노티파이를 통해 호출
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