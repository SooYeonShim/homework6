// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class SPARTAPROJECT_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 SpawnCount = 10;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnRange = FVector(1000.f, 1000.f, 500.f);


	// 에디터에서 어떤 블루프린트를 스폰할지 선택하는 변수
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> MovingClass;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> RotatingClass;

	// 스폰된 액터들을 담아둘 배열
	UPROPERTY()
	TArray<AActor*> SpawnedPlatforms;

	// 타이머 핸들
	FTimerHandle RespawnTimerHandle;

	// 설정값 (에디터에서 수정 가능)
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float RespawnInterval = 5.0f; // 5초마다 재생성

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 핵심 로직 함수들
	void SpawnPlatforms(FBox SpawnArea);   // 실제 스폰 로직 (기존 BeginPlay 내용을 여기로 이동)
	UFUNCTION(BlueprintCallable, Category = "Platform|Actions")
	void CleanupAndRespawn(FBox SpawnArea); // 기존 액터 삭제 후 다시 스폰 호출

};
