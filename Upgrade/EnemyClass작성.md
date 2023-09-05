### 기초 클래스인 ABaseCharacter를 상속받아 AEnemyCharacter를 구현함.
+ #### 모든 적 캐릭터는 AEnemyCharacter를 상속받은 블루프린트로 작성 함.

#### AEnemyCharacter.h

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MagicKnightEnums.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PF_MAGICKNIGHT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class AEnemyAIController* AIController; //AI컨트롤러

	//걷기 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 380.0f;

	//달리기 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 550.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* HitCollision;

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;

public:
	AEnemyCharacter();
};
```

#### AEnemyCharacter.cpp

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	Controller = nullptr;

	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);
	HitCollision->SetCollisionProfileName(TEXT("EnemyHit"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	Controller = Cast<AEnemyAIController>(GetController());
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
```
