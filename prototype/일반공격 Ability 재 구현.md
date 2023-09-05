### 일반공격 Ability 재 구현 및 데미지 판정(04/05)
+ #### 일반공격을 AbilitySystem으로 재작성 하여 구현함.
+ #### 데미지 처리 방식은 스킬과 같은 방식으로 작동

+ #### ![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EC%9D%BC%EB%B0%98%EA%B3%B5%EA%B2%A9%EC%9E%85%EB%A0%A5.PNG?raw=true)
-> 마우스 좌클릭시 이벤트가 호출되어, 현재 무기를 장착 하였는지, 어빌리티 사용 가능한지 확인후, isAttack 을 true로 변경하여 현제 공격중으로 알리고 어빌리티를 작동, 그후 콤보 수치를 +1 함
+ #### ![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EC%BD%A4%EB%B3%B4%EC%A6%9D%EA%B0%80.PNG?raw=true)
-> 콤보수치를 +1 하는 함수 최대 횟수보다 크면 초기화 해주고 아니면 콤보 수치를 +1 함
+ #### ![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EA%B3%B5%EA%B2%A9%EC%96%B4%EB%B9%8C%EB%A6%AC%ED%8B%B0.PNG?raw=true)
-> 실제 공격이 작동 되는어빌리티로 해당 어빌리티가 작동되면, 현재 어빌리티 사용 가능 여부를 확인하고, 현재 콤보를 확인 함
+ #### ![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EA%B3%B5%EA%B2%A9%EC%95%A0%EB%8B%88%EB%A9%94%EC%9D%B4%EC%85%98%EC%A0%9C%EC%96%B4.PNG?raw=true)
-> 현재 콤보 수치에 따라 애니메이션몽타주의 색션을 StartingSection에 저장 
