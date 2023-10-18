#### 원거리Type 적 완성
+ #### AI 비헤이비어 트리
![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EC%9B%90%EA%B1%B0%EB%A6%ACAI.PNG?raw=true)

+ #### 단순히 적이 범위 안에 들업오면 조준 사격을 함.
+ #### 해당 공격은 100%적중, 플레이어는 방어 및 튕겨내기를 하거나, 회피 가능
+ #### 공격을 튕겨내면, 원거리Type 적이 되돌려 받아, 데미지를 받음 -> HP가 매우 낮기 때문에, 1번 튕겨내면 바로 사망

#### 공격 판정 추가
+ #### EAttackCollisionType에 원거리 공격에 해당하는Type 추가 및 Cheack Collision 변경

#### EAttackCollisionType

```cpp
UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"), //기본 근접 공격
	AOE UMETA(DisplayName = "AOE"), //범위 공격
	AOE_Object_Center UMETA(DisplayName = "AOE Object Center"), //특정 객체를 중심으로 범위 공격 (EX: 플레이어의 특수스킬)
	Ramge_Line UMETA(DisplayName = "Range"), //원거리 공격, 시작 지점에서 플레이어 위치 까지

	Max UMETA(DisplayName = "Max")
};
```

#### EnemyCharacter::CheakCollision()

```cpp
FHitResult AEnemyCharacter::CheakCollision(EAttackCollisionType Type, float Range, EDamageEffectType DamageType)
{
	FVector Start = GetMesh()->GetSocketLocation(CollisionStartSocket); //시작 점
	FVector End = GetMesh()->GetSocketLocation(CollisionEndSocket); //끝 점
	FVector TargeLocation; // 원거리 공격에 사용할 플레이어 위치
	APlayerCharacter* Target = Cast<APlayerCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	if (IsValid(Target))
	{
		TargeLocation = Target->GetActorLocation();
	}

	TArray<AActor*> ActorsToIgnore; //판정에서 무시할 객체(자기 자신) 선언 및 추가
	ActorsToIgnore.Add(GetOwner());

	FHitResult OutHit; //Hit결과 구조체
	bool bResult; //Ovelrap 결과

	switch (Type)
	{
	case EAttackCollisionType::Ramge_Line: //원거리 공격 이면
		bResult = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start, 
			TargeLocation,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel3),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true);
		break;
  }
}
```
#### 튕겨내기는 근거리Type 적과 동일하게 GameplayEffect를 제작하고 DamageType을 PostureUp으로 지정하여, 튕겨내기에 성공하면, 데미지가 들어가게 설정 함.
#### GamepalyCue에 OnParryEnemySniper를 추가하여, 바로 사망 모션이 나오도록 구성함.
