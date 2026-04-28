// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class SPARTAPROJECT_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform|Components") // 보이기만 하고 수정은 불가능
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Properties")
	FVector DefaultActorScale = FVector(1.0f, 1.0f, 0.2f);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 움직임 관련 프로퍼티


	// 현재 위치 추적
	FVector StartLocation;
	FVector TargetLocation;
	bool bMovingForward = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveBackAndForth(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Platform|Movement")
	void InitializeMovement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
	float MaxRange = 100.0f;  // 이동할 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
	float MoveSpeed = 50.0f;  // 이동 속도

};
