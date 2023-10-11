#### 플레이어 및 적 캐릭터 UI 구현
+ #### 플레이어 UI는 형태를 변경하고, c++를 이용하여 GameMode에서 설정함.
+ #### 적 캐릭터의 UI는 EnemyCharacter class에 WidgetComponent를 추가하여, 적 캐릭터의 위에 표시되도록 함.


#### 플레이어 캐릭터 UI
![](https://github.com/kimeorua/portfolio/blob/main/img/%ED%94%8C%EB%A0%88%EC%9D%B4%EC%96%B4UI.PNG?raw=true)
+ #### 빨간색 -> 체력
+ #### 파란색 -> EF
+ #### 노란색 -> 체간

#### MagicKnightGameMode.h
```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PF_MagicKnightGameMode.generated.h"

UCLASS(minimalapi)
class APF_MagicKnightGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	//-------------UI----------------//
  //에디터에서 지정할 위젯 블루프린트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> PlayerWidgetClass;

  //c++에서 접근할 위젯 변수
	UPROPERTY()
	class UUserWidget* CurrentWidget;
	//-------------UI----------------//

public:
	void StartPlay() override; //게임 시작시 호출
};
```

#### MagicKnightGameMode.cpp
```cpp
void APF_MagicKnightGameMode::StartPlay()
{
	Super::StartPlay();

 	if (IsValid(PlayerWidgetClass)) //위젯 블루프린트가 유효 하면
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetClass); //해당 위젯을 생성하여, 변수에 저장

		if (IsValid(CurrentWidget))
		{
			CurrentWidget->AddToViewport(); //저장한 위젯이 유효하면, 해당 위젯을 뷰포트에 부착
		}
	}
}
```

#### 적 캐릭터 UI -> 플레이어 위젯과 동일한 색상으로 표기
![](https://github.com/kimeorua/portfolio/blob/main/img/%EC%A0%81UI.PNG?raw=true)
```cpp
#### AEnemyCharacter::AEnemyCharacter
AEnemyCharacter::AEnemyCharacter()
{
	AIController = nullptr;

	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);
	HitCollision->SetCollisionProfileName(TEXT("EnemyHit"));

 //위젯 컴포넌트 생성 및 RootComponent에 부착
	UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI"));
	UI->SetupAttachment(RootComponent);
}
```
