### EP 및 MaxEP 설정, EPSkill(특수 스킬)기초 구현(03/20)
+ ### CharacterAttributeBase에 EP(속성치), MaxEP(최대 속성치)를 지정하여 에디터에서 각각 0, 100으로 초기화 함

```cpp
//속성치 설정
UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_EP)
FGameplayAttributeData EP;
ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, EP)

//최대 속성치 설정
UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_MaxEP)
FGameplayAttributeData MaxEP;
ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxEP)

//속성치 설정
void UCharacterAttributeSetBase::OnRep_EP(const FGameplayAttributeData& OldEP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, EP, OldEP);
}

//최대 속성치 설정
void UCharacterAttributeSetBase::OnRep_MaxEP(const FGameplayAttributeData& OldMaxEP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxEP, OldMaxEP);
}
```

+ ### EPSkills Ability블루프린트를 만들어서, 기초적인 EP수치 확인과, 작동을 구현 함.
![](./img/특수스킬기초.PNG)
