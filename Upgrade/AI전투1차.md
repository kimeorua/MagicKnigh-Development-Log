### AI 중거리 전투 패턴 및 플레이어 추적 구현
+ #### EnemyPettern 열거형을 만들어서, 블루프린트 및 C++에서 사용할수 있도로함.
+ #### RandomPettern_MiddleRange Task를 통해 난수를 설정하여 60%의 확률로 추적을 40%의 확률로 공격 패턴에 들어가도록 구현함.

#### AI비헤이비어트리
![](https://github.com/kimeorua/portfolio/blob/main/img/AI%EC%A0%84%ED%88%AC%EB%B9%84%ED%97%A4%EC%9D%B4%EB%B9%84%EC%96%B4%ED%8A%B8%EB%A6%AC.PNG?raw=true)

+ #### 플레이어가 탐지되면, 플레이와의 거리를 기준으로 패턴을 나눔
+ #### 150 < Distance < 500 이면, RandomPettern_MiddleRange Task를 통하여, 패턴을 정하고, 해당패턴에 맞게 추적 또는 공격에 들어가도록 구성함. 
