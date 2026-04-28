// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
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

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Resources/Materials/M_Rock_Basalt.M_Rock_Basalt"));
    if (MaterialAsset.Succeeded())
    {
        StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
    }
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    MoveSpeed = 100.0f;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
    
    // 시작 위치 저장
    // StartLocation = GetActorLocation();
    // 앞쪽 끝점으로 설정
    // TargetLocation = StartLocation + GetActorForwardVector() * MaxRange;

    InitializeMovement();

    // 2초마다 MoveToRandomLocation 함수 호출
    //GetWorld()->GetTimerManager().SetTimer(
    //    MoveTimerHandle,
    //    this,
    //    &AMovingPlatform::MoveToRandomLocation,
    //    2.0f,
    //    true // 반복 설정
    //);
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    MoveBackAndForth(DeltaTime);
}

void AMovingPlatform::MoveBackAndForth(float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();
    float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);

    // 한 프레임에 이동할 거리 계산
    float MoveStep = MoveSpeed * DeltaTime;

    // 이동할 거리가 MoveStep보다 작을 경우
    if (DistanceToTarget <= MoveStep)
    {
        // 바로 타겟 로테이션으로 이동시킴
        SetActorLocation(TargetLocation);

        // 방향 전환
        if (bMovingForward)
        {
            TargetLocation = StartLocation - GetActorForwardVector() * MaxRange;
            bMovingForward = false;
        }
        else
        {
            TargetLocation = StartLocation + GetActorForwardVector() * MaxRange;
            bMovingForward = true;
        }
    }
    else
    {
        FVector MoveDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
        SetActorLocation(CurrentLocation + MoveDirection * MoveStep);
    }
}

void AMovingPlatform::InitializeMovement()
{
    StartLocation = GetActorLocation();
    // 바뀐 MaxRange를 즉시 반영하여 타겟 계산
    TargetLocation = StartLocation + GetActorForwardVector() * MaxRange;
    bMovingForward = true;
}
