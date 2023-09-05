### EP스킬 제작 및 스킬사용 구현(03/21)
+ #### Weapon class에 EPSkill 몽타주를 변수로 할당하고, EPSkills Ability에서 조건에 부합할시, 해당 무기의, EPSkill을 사용하도록 구현함.

### Ability 사용을위한 조건문 코드 개선 및 방패 효과 추가(03/22)
+ #### Ability 사용을위한 조건문 코드 개선: 기존 블루프린트에서 조건을 확인하는 작업을 C++로 구현하여, 보기 쉽개 정리함.

```cpp 

bool AMainCharacter::CheackCanUseAbility() const //bCanUseAbility를 확인하여 현제 어빌리티 사용 가능 여부를 반환 함
{
	if (bCanUseAbility)
	{
		return true;
	}
	else
	{
		return false;
	}
}

AWeapon* AMainCharacter::CheackCanUseSkillAbility() const // 현제 무기여부 및 Ability사용 여부를 검사하여 현제 무기를 장비중이고 Ability를 사용 가능하면 무기 반환.
{
	if (CheackCanUseAbility() && CurrentWeapon != nullptr)
	{
		return CurrentWeapon;
	}
	else
	{
		return nullptr;
	}
}
```

+ ####  방패 효과 추가: 방어를 사용할려고 할때 방패가 커지는 효과를 추가하여, 기본 상태일때는 작게 방어 상태일때는 커지도록 구현, 사운드 추가.
![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EB%B1%85%ED%8C%A8%ED%9A%A8%EA%B3%BC.gif?raw=true)
