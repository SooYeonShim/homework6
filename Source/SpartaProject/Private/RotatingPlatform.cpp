// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatingPlatform.h"

// Sets default values
ARotatingPlatform::ARotatingPlatform()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Shapes/Shape_Cube.Shape_Cube"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
		StaticMeshComp->SetRelativeScale3D(FVector(DefaultActorScale));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Resources/Materials/M_Metal_Steel.M_Metal_Steel"));
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GetWorld()->GetTimerManager().SetTimer(
	//	DestroyTimerHandle,
	//	this,
	//	&ARotatingPlatform::HandleDestruction,
	//	LifeTime,
	//	false // 반복하지 않음
	//);
	RotationSpeed = 90.0f;

}

// Called when the game starts or when spawned
void ARotatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!FMath::IsNearlyZero(RotationSpeed))
	{
		Rotate(DeltaTime);
	}

}

void ARotatingPlatform::ResetActorPosition()
{
	SetActorLocation(FVector::ZeroVector);
}

void ARotatingPlatform::Rotate(float DeltaTime)
{
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f)); // DeltaTime은 1초에 RotationSpeed만큼 돌아가게 계산해준다.
}

float ARotatingPlatform::GetRotationSpeed() const
{
	return RotationSpeed;
}

void ARotatingPlatform::HandleDestruction()
{
	Destroy();
}
