// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingPlatform.generated.h"

UCLASS()
class SPARTAPROJECT_API ARotatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingPlatform();

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flatform|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Properties")
	FVector DefaultActorScale = FVector(1.0f, 1.0f, 0.2f);


	FTimerHandle DestroyTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Platform|Actions")
	void Rotate(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform|Properties")
	float RotationSpeed;

	UFUNCTION(BlueprintCallable, Category = "Platform|Actions")
	void ResetActorPosition();
	UFUNCTION(BlueprintPure, Category = "Platform|Properties")
	float GetRotationSpeed() const;

	UPROPERTY(EditAnywhere, Category = "Logic")
	float LifeTime = 3.0f;

	// 사라질 때 실행할 함수
	void HandleDestruction();

};
