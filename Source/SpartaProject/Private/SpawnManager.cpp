// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "RotatingPlatform.h"
#include "MovingPlatform.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // C++ 클래스는 메타데이터로 불러오기.
    MovingClass = AMovingPlatform::StaticClass();
    RotatingClass = ARotatingPlatform::StaticClass();

}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

    FBox SpawnArea(GetActorLocation()- SpawnRange, GetActorLocation()+ SpawnRange);

    SpawnPlatforms(SpawnArea);

    // 일정 주기마다 CleanupAndRespawn 함수를 무한 반복 호출
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, [this, SpawnArea](){this->CleanupAndRespawn(SpawnArea);}, RespawnInterval, true);

}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::SpawnPlatforms(FBox SpawnArea)
{
    for (int32 i = 0; i < SpawnCount; i++)
    {
        // 1. 회전 플랫폼 스폰 및 랜덤 설정
        FVector RotLoc = FMath::RandPointInBox(SpawnArea);
        ARotatingPlatform* RotPlat = GetWorld()->SpawnActor<ARotatingPlatform>(RotatingClass, RotLoc, FRotator::ZeroRotator);

        if (RotPlat)
        {
            // 랜덤 회전 속도 및 방향
            float RandomRotSpeed = FMath::RandRange(50.0f, 200.0f) * (FMath::RandBool() ? 1 : -1);
            RotPlat->RotationSpeed = RandomRotSpeed;

            SpawnedPlatforms.Add(RotPlat);
        }

        // 이동 플랫폼 위치 찾기
        FVector MovLoc;
        bool bValidLocation = false;
        int32 Attempts = 0;
        while (!bValidLocation && Attempts < 10)
        {
            MovLoc = FMath::RandPointInBox(SpawnArea);
            if (FVector::Dist(RotLoc, MovLoc) > 300.f) bValidLocation = true;
            Attempts++;
        }

        // 이동 플랫폼 스폰 및 랜덤 설정
        AMovingPlatform* MovPlat = GetWorld()->SpawnActor<AMovingPlatform>(MovingClass, MovLoc, FRotator::ZeroRotator);

        if (MovPlat)
        {
            // 랜덤 속도와 범위 부여
            MovPlat->MoveSpeed = FMath::RandRange(300.0f, 700.0f);
            MovPlat->MaxRange = FMath::RandRange(200.0f, 1000.0f);

            // 랜덤 회전
            FRotator RandomRot = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);
            MovPlat->SetActorRotation(RandomRot);

            // 바뀐 값들을 바탕으로 위치 변수들 다시 계산
            MovPlat->InitializeMovement();

            SpawnedPlatforms.Add(MovPlat);
        }
    }
}

void ASpawnManager::CleanupAndRespawn(FBox SpawnArea)
{

    // 기존에 스폰된 액터들 모두 제거
    for (AActor* Actor : SpawnedPlatforms)
    {
        if (Actor && Actor->IsValidLowLevel())
        {
            Actor->Destroy();
        }
    }
    // 배열 비우기
    SpawnedPlatforms.Empty();

    // 다시 스폰
    SpawnPlatforms(SpawnArea);

    UE_LOG(LogTemp, Warning, TEXT("Platforms Respawned!"));
}

