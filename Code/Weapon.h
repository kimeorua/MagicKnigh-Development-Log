// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicKnightEnums.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Q_Skill UMETA(DisplayName = "Q_Skill"), //�߰� ������
	E_Skill UMETA(DisplayName = "E_Skill"), //���� ������
	Special_Skill UMETA(DisplayName = "Special_Skil"), //���� ������

	Max UMETA(DisplayName = "Max")
};

UCLASS()
class PF_MAGICKNIGHT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UParticleSystem* GetSoummonsParticle() const { return SoummonParitcle; }
	FORCEINLINE USoundBase* GetSoummonSound() const { return SoummonSound; }
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }

	//������ Ȯ��
	UFUNCTION(BlueprintCallable)
	FHitResult CheakCollision(EAttackCollisionType Type, float Range, EAttackDirectionType DriectionType, EDamageEffectType DamgeType);

	//�� �迭 �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
	void HitArrReset();

	/// <summary>
	/// ���� ��Ÿ�� ��ȯ
	/// </summary>
	/// <param name="index">���� ��ȣ</param>
	/// <returns>���� �ִ� ��Ÿ��</returns>
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetCurrentAttackMontage(int index) const;

	/// <summary>
	/// ��ų ��Ÿ�� ��ȯ
	/// </summary>
	/// <param name="Type">��ų Ÿ��</param>
	/// <returns>��ų �ִ� ��Ÿ��</returns>
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetCurrentSkillMontage(ESkillType Type) const;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoummonParticle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* SoummonParitcle = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoummonParticle", meta = (AllowPrivateAccess = "true"))
	USoundBase* SoummonSound = nullptr;

	// ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRadius = 0.f;

	//Trace Start Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionStartSocket = "";

	//Trace End Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionEndSocket = "";

	//Trace AOE Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FName CollisionAOESocket = "";

	//���ݿ� ���� �� �迭
	TArray<class AEnemyCharacter*>HitEnemys;

	class UCombetComponent* CombetComp;

	/// <summary>
	/// �޺� ���� ��Ÿ��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackMontages;

	/// <summary>
	/// ��ų ��Ÿ��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TMap<ESkillType, UAnimMontage*> SkillMontages;
};
