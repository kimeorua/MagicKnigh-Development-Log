// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
class AShield; //방패 class
class AWeapon; //무기 class

UCLASS()
class MYGAME_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AMainCharacter();

	bool GetUseBlock() { return bUseBloack;  } //방어 중인지 확인 하는 변수 -> 애니메이션 제어를 위해 Get만 구현

private:

	//카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	//-----------------------------------------------------------------------------------------------------//
	
	//이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float ForwardWalkSpeed = 350.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float ForwardRunSpeed = 650.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float BackwardSpeed = 100.f;

	float CurrentSpeed; //현제 이동 속도
	//-----------------------------------------------------------------------------------------------------//

	bool bUseDash; //달리기 사용 여부

	//방어 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bUseBloack;

	//어빌리티 사용 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bCanUseAbility;

	//현재 어빌리티 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bUseAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool bIsHit;

	//이동 방향용 변수(회피 애니메이션 제어 용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	int32 MoveNum;

	//현제 공격 중인지 확인(C++ 전용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool IsAttack;

	//현제 회피 중인지 판단(DodgeControll 애님노티파이 스테이트에서 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet", meta = (AllowPrivateAccess = "true"))
	bool IsDodge;

	//애님인스턴스 변수
	class UMainAnimInstance* MainAnimInstance;

	//방패
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AShield> ShieldClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	AShield* Shield = nullptr;
	//-----------------------------------------------------------------------------------------------------//

	//검
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> SwordClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	AWeapon* Sword = nullptr;

	//검
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> AxeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	AWeapon* Axe = nullptr;
	//-----------------------------------------------------------------------------------------------------//

	//현재 사용 무기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	AWeapon* CurrentWeapon = nullptr;
	
	//현재 사용 하는 무기의 고유 번호(애니메이션 제어)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combet, meta = (AllowPrivateAccess = "true"))
	int32 CurrentWeaponNum= 0;

	//-----------------------------------------Function-----------------------------------------//

	//기본 움직임
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	//-----------------------------------------------------------------------------------------------------//

	//달리기
	void Dash();
	void DashEnd();
	//-----------------------------------------------------------------------------------------------------//

	//회피
	void Dodge();

	// 무기 장착 함수--블루프린트에서 호출 가능 하게 구현
	UFUNCTION(BlueprintCallable)
	bool Equip(AWeapon* UseWeapon, FName EquipSocket, int32 EquipNum);
		
	//카메라 관련 컴포넌트 반환용 함수
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void PlayerOnHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 무기 고유 번호 반환용 함수
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentWeaponNum() const { return CurrentWeaponNum; }

	//무기 반환용 함수
	FORCEINLINE class AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintCallable)
	bool CheackCanUseAbility() const;

	UFUNCTION(BlueprintCallable)
	AWeapon* CheackCanUseSkillAbility() const;

	void AttackEnd(); // 공격 종료 함수
	void CheackCombo(); // 콤보 체크용 함수
};
