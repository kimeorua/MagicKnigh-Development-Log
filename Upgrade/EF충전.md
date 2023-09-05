### EF 충전 구현
+ #### 이전에 사용한 체력감소, 체간 상승과 같은 방식으로 GameplayEffect를 작동 시킴

```cpp
void APlayerCharacter::EFCharge()
{
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle;
	SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(HitEffects[3], 1, EffectContext);

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
```
