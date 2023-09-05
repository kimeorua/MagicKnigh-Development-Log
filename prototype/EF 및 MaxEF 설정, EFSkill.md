### EF 및 MaxEF 설정, EFSkill(특수 스킬)기초 구현(03/20)
+ ### CharacterAttributeBase에 EF(속성치), MaxEF(최대 속성치)를 지정하여 에디터에서 각각 0, 100으로 초기화 함

```cpp
//속성치 설정
UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_EF)
FGameplayAttributeData EF;
ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, EF)

//최대 속성치 설정
UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_MaxEF)
FGameplayAttributeData MaxEF;
ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxEF)

//속성치 설정
void UCharacterAttributeSetBase::OnRep_EF(const FGameplayAttributeData& OldEF)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, EF, OldEF);
}

//최대 속성치 설정
void UCharacterAttributeSetBase::OnRep_MaxEF(const FGameplayAttributeData& OldMaxEF)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxEF, OldMaxEF);
}
```

+ ### EFSkills Ability블루프린트를 만들어서, 기초적인 EF수치 확인과, 작동을 구현 함.
![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%ED%8A%B9%EC%88%98%EC%8A%A4%ED%82%AC%EA%B8%B0%EC%B4%88.PNG?raw=true)
