#### 보스Type 적 캐릭터 생성 및 기본 설정 셋팅
+ #### 보스Type의 적 캐릭터의 기본 GameplayEffect 및 Attribute를 설정함.
+ #### 보스Type의 체력 및 체간 UI는 보스가 플레이어를 인식하면 Add to Viewport를 통해 출력함 -> 기존의 적 UI와는 다른 형식으로 제작.

#### 보스Type 적 캐릭터
![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EB%B3%B4%EC%8A%A4%EC%BA%90%EB%A6%AD%ED%84%B0.PNG?raw=true)

#### UI
![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EB%B3%B4%EC%8A%A4UI.PNG?raw=true)

+ #### 플레이 화면의 상단에 표시 됨.

#### 보스 UI 출력
![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EB%B3%B4%EC%8A%A4UI%EC%B6%9C%EB%A0%A5.PNG?raw=true)

+ #### 기존의 UI컴포넌트를 사용하지 않고, 블루프린트에서, UI class를 변수로 추가하여 설정함.
+ #### 해당 UI Class를 생성하고, Owner 설정한 후, Add to Viewport통해 뷰포트에 추가함.
+ #### EnemyAIController class에서 bShowUI변수를 추가하여 OnTargetDetected() 함수에서, 현재 보스Type의 적 이면, 플레이어 탐지 시, bShowUI변수를 true로 변경 함 그 후 하단의 블루프린트를 통하여, Visibility를 설정 함.
+ ![](https://github.com/kimeorua/kimeorua.github.io/blob/main/img/%EB%B3%B4%EC%8A%A4UI%EC%B6%9C%EB%A0%A5%EC%97%AC%EB%B6%80.PNG?raw=true)
