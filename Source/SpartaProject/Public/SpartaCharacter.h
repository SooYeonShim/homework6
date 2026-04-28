// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue; // 구조체

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpartaCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UCameraComponent* CameraComp;

private:
	float NormalSpeed; // 기본 스피드
	float SprintSpeedMultiplier; // 기존 스피드에 곱해주는 수
	float SprintSpeed; // 실제로 곱해준 대시 스피드

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // IA 액션해다가 함수들을 연결해줄 함수

	// 입력 처리 시스템은 리플렉션 시스템과 연동이 되어있어야한다.
	UFUNCTION()
	void Move(const FInputActionValue& value); // FInputActionValue는 IA의 Value Type 값을 받겠다는 뜻. const 참조로 받는 이유는 복사 비용을 줄이기 위해서이다.
	UFUNCTION()
	void StartJump(const FInputActionValue& value); // bool 타입은 시작하는 동작과 멈추는 동작을 나눠주는 것이 좋음
	UFUNCTION()
	void StopJump(const FInputActionValue& value); //
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	


};
