#### 정예Type 적 캐릭터 완성
+ #### 정예Type의 적 패턴을 완성함.
+ #### 패턴1: 느린 공격2번 빠른 공격2번을 하는 연속 공격
+ #### 패턴2: 빠른공격 4번후, 방어불가 공격1번을 하는 연속 공격
+ #### 패턴3: 느린 공격 3번하는 연속 공격
+ #### 점프공격: 플레이어와의 거리가 일정 수준 벌어지면 플레이어에게 점프하여, 공격 함.

#### AI 비헤이비어 트리
![]()

#### 적 캐릭터CheakCollision() 함수 변경
+ #### 정예Type의 적 캐릭터는 양손에 무기를 각각 1개씩 들고 있음
+ #### 따라서, 공격 시 왼손, 오른손인지 확인하여, 충돌 시작점을 구분 함.

```cpp
FHitResult AEnemyCharacter::CheakCollision(EAttackCollisionType Type, float Range, EDamageEffectType DamageType, EAttackDriectionType DriectionType)
{
	FVector Start ; //시작 점
	FVector End; //끝 점

	FVector TargtLocation;

	switch (DriectionType) //현재 공격하는 방향이 왼쪽 or 오른쪽인지 판단하여, 충돌 지점을 다르게 함
	{
	case EAttackDriectionType::None:
		Start = GetMesh()->GetSocketLocation(CollisionStartSocket_L); //시작 점
		End = GetMesh()->GetSocketLocation(CollisionEndSocket_L); //끝 점
		break;

	case EAttackDriectionType::Left:
		Start = GetMesh()->GetSocketLocation(CollisionStartSocket_L); //시작 점
		End = GetMesh()->GetSocketLocation(CollisionEndSocket_L); //끝 점
		break;

	case EAttackDriectionType::Right:
		Start = GetMesh()->GetSocketLocation(CollisionStartSocket_R); //시작 점
		End = GetMesh()->GetSocketLocation(CollisionEndSocket_R); //끝 점
		break;

	case EAttackDriectionType::Max:
		break;

	default:
		break;
	}

```
