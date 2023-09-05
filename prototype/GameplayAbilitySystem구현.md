### GameplayAbilitySystem 구현 (03/07)
+ 프로젝트에 GameplayAbilitySystem플러그 인을 사용하여, 기초적인 Attribute, Ability, GameplayAbilityComponent를 구현하였음 그후 BaseCharacter class에 상속함
+ #### 참고 한 영상 https://www.youtube.com/watch?v=Yub52f4ZUU0&t=1580s
+ #### 참고 한 예시 프로젝트: UE4.27ver 액션RPG

CharacterAttributeSetBase
```cpp

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MYGAME_API UCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	//현제 HP
	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Health)

	//최대 HP
	UPROPERTY(BlueprintReadOnly, Category = "Abilities", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxHealth)


	//-------------------------------------------Funtion----------------------------------------------------//
	UCharacterAttributeSetBase();
	//필수 정의 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const  override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth); // HP 정의
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth); //MaxHP 정의
};
```
CharacterGameplayAbility

 ```cpp
UCLASS()
class MYGAME_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCharacterGameplayAbility();
	//마우스&키보드 입력 지정
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EGameAbilityInputID AbilityInputID  = EGameAbilityInputID::None;
};
```
