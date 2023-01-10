# MakeGame

# 졸업 작품 및 포트폴리오 정리

## 목차
+ ### [1.게임 설명](#게임-설명)
+ ### [2.클래스 구조](#클래스-구조)
+ ### [3.개발 요약](#개발-요약)
+ ### [4.개발 사항](#개발-및-작성-사항)

## 게임 설명
+ ### 모티브: God Of War & Sekiro
+ ### God Of War: 공격 및 피격, 스킬사용 등에서 오는 짜릿한 타격감을 구현 할 예정
+ ### Sekiro: 패링위주의 전투를 구상해볼 예정
+ ### 장르: 액션RPG
+ ### 제작툴 및 언어: 언리얼엔진5 & C++
+ ### 한 줄 설명: 스테이지를 진행하면서 보스를 잡고 무기의 및 새로운 기술을 해금 하여 스태이지를 진행 그후 최종 보스를 잡는 형식의 게임

## 클래스 구조

### 클래스 상속
![](./img/클래스상속도.PNG)

## 개발 요약

### 12/26 ~ 12/30 

+ ### 12/26 작성 및 Github 작성
+ ### 12/27 메인 캐릭터 기능 및 요소 구상
+ ### 12/28 일반 몬스터 기능 및 요소 구상
+ ### 12/29 보스 몬스터 기능 및 요소 구상 && Udemy 강좌 정리
+ ### 12/30 무기 기능 및 요소 구상

### 01/02 ~ 01/06

+ ### 01/02 아이템 클래스 구상 및 무기클래스 
+ ### 01/03 ~ 01/04 메인캐릭터 이동 및 시점 구현
+ ### 01/06 ~ 01/08 개발 중지 및 프로잭트 재생성
+ ### 01/09 ~ 01/10 캐릭터 이동 재 구현

## 개발 및 작성 사항

---

### 1.메인 캐릭터 (12/27)
#### --------------------------요소---------------------------
+ #### 현재 상태(이동, 방어, 공격, 스킬 사용, etc…..)
+ #### HP(생명력)
+ #### EP(속성력 -> 공격 or 패링시 상승, 꽉 차면 특수 스킬 사용)
+ #### 스킬 쿨 타임(Q, E 각 스킬마다 다른 쿨 타임 가짐)
+ #### ES(강화 재료)
#### --------------------------기능----------------------------
+ #### W(앞), A(좌), S(뒤), D(우)
+ #### 왼쪽Shift(대쉬)
+ #### 왼쪽마우스 클릭(공격 -> 연속으로 클릭하면 콤보 공격 가능)
+ #### 오른쪽마우스 클릭(방어 & 패링)
+ #### 숫자1, 2(무기 변경 키)
+ #### 왼쪽Ctrl(특수 사용)
+ #### 스페이스바(회피)
+ #### Q, E(일반 스킬)
+ #### R(회복 포션 사용)
+ #### I(정비창)
+ #### F(상호 작용)


### 2.일반 몬스터(12/28)
#### --------------------------요소---------------------------
+ #### 현재 상태(이동, 공격, 사망, etc…..)
+ #### HP(생명력)
+ #### StackEP(속성력 누적치-> 플레이어의 공격에 피격 시 or 플레이어가 패링 성공 시 상승)
+ #### SturnTime(기절 시간)
+ #### Type(몬스터의 종류, 속성이나 근or원거리 등등)
#### --------------------------기능----------------------------
+ #### AI(공격, 대기, 플레이어에게 이동, etc…….)
+ #### 아이템드롭(회복 포션 or 강화 재료)

### 3-1.보스 몬스터 : 일반몬스터(상속) (12/29)
#### --------------------------요소---------------------------
+ #### 현재 상태(이동, 공격, 사망, etc…..)
+ #### HP(생명력)
+ #### StackEP(속성력 누적치-> 플레이어의 공격에 피격 시 or 플레이어가 패링 성공 시 상승)
+ #### SturnTime(기절 시간)
+ #### Type(몬스터의 종류, 속성이나 근or원거리 등등)
+ #### EP(특수 패턴용 속성치, 플레이어에게 피격or 공격 시 증가)
+ #### Pase(보스 페이즈 남은 HP 따라 변경)
#### --------------------------기능----------------------------
+ #### AI(공격, 대기, 플레이어에게 이동, etc…….)
+ #### 아이템드롭(회복 포션 or 강화 재료)

### 3-2.Udemy 강좌 정리

#### 강좌 명: Unreal Engine 5 C++ Developer: Learn C++ & Make Video Games

#### 챕터: HealthComponent

내용 정리: 여태까지 플레이어 캐릭터, 적캐릭터 등, HP를 가지고 있는 경우 해당 클래스에서 구현하여 클래스가 길어 졌는데, 해당 강좌에서 설명해준 ActorComponent를 상속 받아
HealthComponent를 C++로 작성하고 각 객체에 추가하여 프로젝트를 제작하는 방법을 습득, 이번에 만드는 게임에서도 사용해볼 예정

사용 법: HealthComponent를 개량하여 현제 제작할 게임에 맞게 변경 -> HP뿐만아니라 EP(속성치)나, 기절 시간 등 전투에 사용될 공통 부분을 제어할 CombetComponent를 작성 하도록 함.

__TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamgeType, AController* Instigator, AActor* DamageCauser)__

_DamagedActor: 데미지를 받는 액터_

_Damage: 받은 데미지_

_DamgeType: 데미지 타입 (후에 필요한 타입을 정리하여, 추가 해볼 예정)_

_Instigator: 데미지를 준 객체의 컨트롤러_

_DamageCauser: 데미지를 준 객체_

해당 함수를 사용하는 것도 좋아보임 고려해볼것

### 4.PickUpItem (01/02)
#### --------------------------기능---------------------------
+ ### PickUp(특정 거리에 플레이어가 오면 상호작용 키가 보이며 해당키를 누르면 PickUp)

### 5.무기 (01/02)
#### --------------------------요소---------------------------
+ #### Skill(무기에 장착된 스킬, 2개)
+ #### ESkill(속성치가 다 차면 사용 가능한 스킬)
+ #### Damage(공격력)
#### --------------------------기능---------------------------
+ #### 콜리전On/Off(공격 시작 시 콜리전On 종료 시 Off)
+ #### 무기 해금 기능(PickUp시 호출 -> 무기 해금)

### 6.자동사용 아이템 : PickUpItem (01/02)
#### --------------------------요소---------------------------
+ #### HealingType(HP-체력 or EP-속성력)
+ #### HealingAmount(회복량)
#### --------------------------기능---------------------------
+ #### 회복(PickUp 시 자동으로 호출-> Type에 따라서 체력(HP) or 속성력(EP) 회복)

---

### 1.캐릭터 이동 및 시점 구현

+ #### 이동 방식: W, S, A, D키를 이용하여 앞, 뒤, 좌, 우 방향으로 이동 하도록 구현

```cpp
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward", this, &AMainCharacter::MoveForward); 	//앞, 뒤 이동
	PlayerInputComponent->BindAxis("Move Right", this, &AMainCharacter::MoveRight);		//좌, 우 이동
	PlayerInputComponent->BindAxis("Turn Right", this, &AMainCharacter::AddControllerYawInput); //시점 좌, 우
	PlayerInputComponent->BindAxis("Turn Up", this, &AMainCharacter::AddControllerPitchInput);  //시점 상, 하
}
void AMainCharacter::MoveForward(float Value) //앞,뒤 이동 뒤로 이동할시 이동속도 감소
{
	if (Value < 0.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = BackworadWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = ForwardWalkSpeed;
	}
	AddMovementInput(GetActorForwardVector() * Value);

}

void AMainCharacter::MoveRight(float Value) //좌, 우 이동
{
	AddMovementInput(GetActorRightVector() * Value);
}

```

+ #### 주요 함수

```cpp
AddMovementInput(FVector) // 해당 백터의 방향으로 움직임 줌
AddControllerPitchInput() // Pitch 방향으로 회전 줌
AddControllerYawInput() //Yaw 방향으로 회전 줌
PlayerInputComponent->BindAxis("이벤트 이름", this, Function) // 축 입력 이벤트에 대하여 해당 함수를 Bind함
GetActorForwardVector() // Actor의 앞방향 벡터를 반환
GetActorRightVector() // Actor의 오른쪽 방향 백터를 반환
```

---

### 프로젝트 중단 및 재 생성

+ #### 문제1 현제 사용할려는 애니메이션과 캐릭터 에셋간의 스켈레톤 구조가 매우 다름, 리타겟팅을 써도 사용 불가, 새로운 캐릭터 에셋 및 애니메이션을 사용 해야 함,
+ #### 문재: 현제 프로젝트의 폴더 정리가 매우 난잡하고 파일의 크기가 매우 커짐

### 해결

+ #### 사용할려는 애니메이션에 제일 비슷한 구조를 가진 pragon에셋을 사용하도록 결정, 프로젝트 정리 및 파일 크기 압축을 위해 애니메이션 작업용 프로젝트를 따로 만들어 애니 메이션을 먼져 작업후 본 프로젝트에 추가하여 작업을 진행 함

### 캐릭터 이동 재구현

+ #### 이동 방식: W, S, A, D키를 이용하여 앞, 뒤, 좌, 우 방향으로 이동 하도록 구현

```cpp
void AMainCharacter::MoveForward(float Value)
{
	if (Value > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	}
	else if (Value < 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed - BackwardSpeed;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
```
#### 설명: 이동방향 맡게 이동 시키며 후방 이동 시 BackwardSpeed 만큼 이동속도를 감소 시킴

+ #### 대쉬: LShift 키를 누르고 있으면 대쉬를 하도록 구현

```cpp
void AMainCharacter::Dash()
{
	if (!bUseDash)
	{
		bUseDash = true;
		CurrentSpeed = ForwardRunSpeed;
	}
}

void AMainCharacter::DashEnd()
{
	if (bUseDash)
	{
		bUseDash = false;
		CurrentSpeed = ForwardWalkSpeed;
	}
}
```
#### 설명: 키 입력을 받아 Shift키가 눌리면 이동속도를 ForwardRunSpeed(뛰기 속도)로 변경하고 눌리지 않았을 경우 ForwardWalkSpeed 걷기 속도로 변경 함
