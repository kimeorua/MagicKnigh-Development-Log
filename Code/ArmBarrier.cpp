// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmBarrier.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AArmBarrier::AArmBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AArmBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArmBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//방패 크기 증가
void AArmBarrier::BarrierOn()
{
	if (!IsBarrierOnEffect)
	{
		IsBarrierOnEffect = true;
		Mesh->SetRelativeScale3D(FVector(0.5f));

		if (BarrierOnSound == nullptr) { return; }
		UGameplayStatics::PlaySound2D(this, BarrierOnSound);
	}
}

// 방패 크기 원복
void AArmBarrier::BarrierOff()
{
	if (IsBarrierOnEffect)
	{
		Mesh->SetRelativeScale3D(FVector(0.25));

		if (BarrierOffSound == nullptr) { return; }
		UGameplayStatics::PlaySound2D(this, BarrierOffSound);
	}
	IsBarrierOnEffect = false;
}
