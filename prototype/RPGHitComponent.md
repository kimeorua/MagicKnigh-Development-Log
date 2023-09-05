### RPGHitComponent(충돌 시스템)제작(02/14)
+ #### BoxComponent를 상속 받아 무기에 적용할 충돌 시스템인 RPGHitComponent 제작
+ #### 애니메이션에서 공격 시작 및 종료시 Collision을 On/Off 하도록 구현

#### RPGHitComponent.h

```cpp
private:
	URPGHitComponent(); //생성자

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); //overlap판정
```

#### RPGHitComponent.cpp

```cpp
URPGHitComponent::URPGHitComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // Tick 함수 안 쓰니까 false로.

	OnComponentBeginOverlap.AddDynamic(this, &URPGHitComponent::OnOverlapBegin); //OnComponentBeginOverlap이벤트에 OnOverlapBegin함수를 바인드함.
}

void URPGHitComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMainCharacter::StaticClass())) // Overlap한 Actor가 MainCharacter이면 무시
	{
		return;
	}
	else		// 그 외의 객체면 Overlap하면 log를 띄움
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit!!"));
	}
	
}

void URPGHitComponent::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionEnabled(ECollisionEnabled::NoCollision); // 게임 시작시 콜리전off
}
```

UMainAnimInstance.cpp

```cpp
void UMainAnimInstance::AnimNotify_AttackEnd() //공격 종료 -> Collision off
{
	Main->AttackEnd();
	URPGHitComponent* RPGHitComponent = Main->GetCurrentWeapon()->GetRPGHitComponent(); // 무기의 RPGHitComponent를 저장
	RPGHitComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Collision off
}

void UMainAnimInstance::AnimNotify_CheackCombo() // 콤보 공격 체크 -> 다음 콤보 공격을 하기전 Collision off 를하여, 추가적인 Overlap 방지
{
	Main->CheackCombo();
	URPGHitComponent* RPGHitComponent = Main->GetCurrentWeapon()->GetRPGHitComponent(); // 무기의 RPGHitComponent를 저장
	RPGHitComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Collision off
}

void UMainAnimInstance::AnimNotify_CollisionStart() // 공격 시작 -> Collision On
{
	URPGHitComponent* RPGHitComponent = Main->GetCurrentWeapon()->GetRPGHitComponent(); // 무기의 RPGHitComponent를 저장
	RPGHitComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); Collision On
}
```
