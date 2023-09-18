### 적 캐릭터 피격 시 Tag 부착 및 제거
+ #### 현재 적 캐릭터의 공격 패턴은, 특정 GameplayTag의 유무에 따라 실행 여부가 결정됨,
+ #### Enemy.State.Attack(현재 공격 중)Tag는 어빌리티가 작동되면, 바로 부착되어, 어빌리티의 연속 작동을 막고 있음
+ #### 피격 시에도 적 캐릭터의 공격을 끊고, 피격 모션이 끝날동안 공격 할수 없게 만들어야 함.
+ #### 피격 시 AnimNotify를 이용하여 Enemy.State.Hit Tag를 부착 및 제거 하도록 함.

#### AN_HitTagOm
![](https://github.com/kimeorua/portfolio/blob/main/img/HitTagOn.PNG?raw=true)

#### AN_HitTagOff
![](https://github.com/kimeorua/portfolio/blob/main/img/HitTagOff.PNG?raw=true)
