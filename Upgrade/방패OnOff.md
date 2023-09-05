### 방어가 작동하면 방패가 커지고, 작동이 중지되면 원래 크기로 돌아가는 함수를 제작.
+ #### 방패가 커지는 BarrierOn()함수는 방어 Ability가 실행되면 호출될수 있도록 BlueprintCallAble로 작성하고, 방패가 원복하는 BarrierOff()함수는 플레이어 캐릭터의 BlockEnd()에서 호출함.

#### AArmBarrier.cpp
```cpp
void AArmBarrier::BarrierOn()
{
	if (!IsBarrierOnEffect)
	{
		IsBarrierOnEffect = true;
		Mesh->SetRelativeScale3D(FVector(0.5f));

		if (BarrierOnSound == nullptr) { return; }
		UGameplayStatics::PlaySound2D(this, BarrierOnSound);
	}
}

void AArmBarrier::BarrierOff()
{
	if (IsBarrierOnEffect)
	{
		Mesh->SetRelativeScale3D(FVector(0.25));

		if (BarrierOffSound == nullptr) { return; }
		UGameplayStatics::PlaySound2D(this, BarrierOffSound);
	}
	IsBarrierOnEffect = false;
}
```
#### APlayerCharacter.cpp
```cpp
// 방어 종료
void APlayerCharacter::BlockEnd()
{
	if (GetAbilitySystemComponent()->GetTagCount(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock"))) > 0)
	{
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UseBlock")));
		ArmBarrier->BarrierOff(); //방패 크기 원복
	}
}
```
#### BlockAbility
![](https://github.com/kimeorua/portfolio/blob/main/img/GA_BlockFull.PNG?raw=true)
