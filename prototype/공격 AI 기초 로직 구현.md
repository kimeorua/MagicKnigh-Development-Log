### 공격 AI 기초 로직 구현(04/11)
+ #### 공격 및 근접 패턴에서 사용할 AI의 기초 로직을 구현함 -> 현제는 공격 범위 안에 들어오면 자동으로 공격을 1번 하도록 간단하게 구현함
+ #### Enemy 클래스에 일반 공격에 해당하는 ComboAttackMontage와 근접 패턴인 MeleePatternMontage변수를 추가하고 에디터에서 애니메이션 지정

![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EA%B3%B5%EA%B2%A9AI.PNG?raw=true)

->AI에서 공격을 호출하면 Enemy_BP에서 커스텀 이벤트인 AttackHandle을 호출함, 그후 공격 종료 Message가 올때 까지 대기

![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EC%A0%81%EA%B3%B5%EA%B2%A9%EC%9E%91%EB%8F%99.PNG?raw=true)

-> 호출되면 현제 블랙보드에 공격중을 표시하고 Enemy.Attack 태그를 가진 어빌리티를 작동 시킴

![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EA%B3%B5%EA%B2%A9%EC%95%A0%EB%8B%98%EC%8A%A4%ED%85%8C%EC%9D%B4%ED%8A%B8.PNG?raw=true)

-> 공격 애니메이션의 끝부분에서 블랙보드에 공격 중을 false로 지정하고 공격 종료 Message를 보냄

+ #### 공격Ability는 메인캐릭터외 유사하여 생략 함.
