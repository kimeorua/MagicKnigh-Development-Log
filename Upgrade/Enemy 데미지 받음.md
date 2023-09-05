### 적 캐릭터 피격 시 체력 감소 구현
+ #### 해당 함수는 Weapon의 CheakCollision()함수에서 적중시 호출 됨.
+ #### HitEffects는 AEnemyCharacter.h에 변수로 설정되어, 여러 상황에 맞는 HitEffect를 에디터상에서 설정함.

#### AEnemyCharacter.cpp

```cpp
void AEnemyCharacter::TakeDamgeFormPlayer()
{
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext(); //EffectContext()생성
	EffectContext.AddSourceObject(this);// 해당 Effect가 생성된 객체 설정
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[0], 1, EffectContext); //SpecHandle 설정

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()); //SpecHandle이 유효하면, 해당 Effect를 작동함.
	}
}
```
