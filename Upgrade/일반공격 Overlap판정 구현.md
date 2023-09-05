### 공격의 판정을 구현함.

#### Weapon.cpp
+ #### HitEnemys는 적중한 적을 배열에 넣고, 이미 배열에 들어간 적은 그 공격에 연속으로 타격받지 않게 함.
+ #### HitEnemys는 게임 시작과 동시에 초기화 됨
+ #### 피격 판정은 LineTraceSingle()함수를 통해 확인함.
+ #### CheakCollision()함수는 BluerprintCallable함수로 AnimationNotify에서 호출하여, 공격 애니메이션이 재생 되는 중, 공격이 들어가는 부분에서만 작동 되도록 함.
+ #### HitArrReset()함수는 공격 판정이 끝나는 부분에서 호출 되도록 함.

```cpp
FHitResult AWeapon::CheakCollision(EAttackCollisionType Type, float Range = 0.f)
{
	FVector Start = Mesh->GetSocketLocation(CollisionStartSocket); //시작 점
	FVector End = Mesh->GetSocketLocation(CollisionEndSocket); //끝 점

	TArray<AActor*> ActorsToIgnore; //무시할 객체
	ActorsToIgnore.Add(GetOwner());//Owner(플레이어 캐릭터)추가
	FHitResult OutHit; //Hit 결과 구조체
	bool bResult; // 판정 성공여부

	bResult = UKismetSystemLibrary::LineTraceSingle(
	GetWorld(),
	Start,
	End,
	UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2),
	false,
	ActorsToIgnore,
	EDrawDebugTrace::ForDuration,
	OutHit,
	true);

	if (bResult)
	{
		if (HitEnemys.IsEmpty())
		{
			HitEnemys.Add(Cast<AEnemyCharacter>(OutHit.GetActor()));
			UE_Log(LogTemp, Warning, TEXT("EnelyHit"));
		}
		else if(!HitEnemys.IsEmpty())
		{
			if (!HitEnemys.Contains(OutHit.GetActor()))
			{
				AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(OutHit.GetActor());
				UE_Log(LogTemp, Warning, TEXT("EnelyHit"));
				HitEnemys.Add(HitEnemy);
			}
		}
	}
	return OutHit;
}

void AWeapon::HitArrReset()
{
	HitEnemys.Empty();
}
```
