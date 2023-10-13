#### 플레이어 사망UI 및 일시정지UI 구현
+ #### 플레이어가 사망하면 사망UI를 띄움
+ #### 일시정지 키를 누르면 게임이 일시 정지 되고, UI가 나옴

#### 플레이어 사망UI
![](https://github.com/kimeorua/portfolio/blob/main/img/%ED%94%8C%EB%A0%88%EC%9D%B4%EC%96%B4%EC%82%AC%EB%A7%9DUI.PNG?raw=true)

#### 게임 일시정지UI
![](https://github.com/kimeorua/portfolio/blob/main/img/%EC%9D%BC%EC%8B%9C%EC%A0%95%EC%A7%80UI.PNG?raw=true)

#### PlayerCharacter::GamePause
```cpp
void APlayerCharacter::GamePause()
{
	PauseWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PauseWidgetClass); //블루프린트에서 지정한 위젯 생성 및 PauseWidget에 저장

	UGameplayStatics::SetGamePaused(GetWorld(), true); //게임일시정지
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true); //마우스 커서 보이도록 설정
	PauseWidget->AddToViewport(); //위젯 뷰포트에 추가
}
```
