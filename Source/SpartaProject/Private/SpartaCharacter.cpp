// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h" // 캐릭터 이동 컴포넌트

// Sets default values
ASpartaCharacter::ASpartaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent); // 루트컴포넌트는 액터의 멤버 변수이며 캐릭터 클래스에서는 생성자에서 캡슐 컴포넌트를 갖게 된다.
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true; // 마우스 컨트롤 회전은 스프링 암에서 켜기

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName); // 소켓이 스프링 암의 끝을 가리킨다.
	CameraComp->bUsePawnControlRotation = false; // 카메라에서는 마우스 컨트롤 회전 끄기

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}


// Called to bind functionality to input
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 플레이어가 받은 입력을 처리
	// 향상된 인풋 컴포넌트로 캐스팅을 한번 해주어야함.
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// GetController를 가져오고, 그것을 직점 만든 컨트롤러로 캐스팅해준다.
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction) // MoveAction 가져오기
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction, // 첫 번째 인자: 이동 입력 IA 가져오기
					ETriggerEvent::Triggered, // 두 번째 인자: 키가 눌렸을 때, 트리거
					this, // 세 번째 인자: 호출된 함수의 객체
					&ASpartaCharacter::Move // 네 번째 인자: Move 함수를 연결
				);
			}
			if (PlayerController->JumpAction) // JumpAction 가져오기 / Start, Stop 둘 다 이 안에 있음
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction, // 첫 번째 인자: 점프 입력 IA 가져오기
					ETriggerEvent::Triggered, // 두 번째 인자: 키가 눌렸을 때 트리거
					this, // 세 번째 인자: 호출된 함수의 객체
					&ASpartaCharacter::StartJump // 네 번째 인자: StartJump 함수를 연결
				);
				EnhancedInput->BindAction(
					PlayerController->JumpAction, // 첫 번째 인자: 점프 입력 IA 가져오기
					ETriggerEvent::Completed, // 두 번째 인자: 점프가 완료되었을 때 트리거
					this, // 세 번째 인자: 호출된 함수의 객체
					&ASpartaCharacter::StopJump // 네 번째 인자: StopJump 함수를 연결
				);
			}
			if (PlayerController->LookAction) // LookAction 가져오기
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction, // 첫 번째 인자: 시점이동 입력 IA 가져오기
					ETriggerEvent::Triggered, // 두 번째 인자: 키가 눌렸을 때 트리거
					this, // 세 번째 인자: 호출된 함수의 객체
					&ASpartaCharacter::Look // 네 번째 인자 Look 함수를 연결
				);
			}
			if (PlayerController->SprintAction) // 스프린트 Action 가져오기 / Start, Stop 둘 다 이 안에 있음
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction, // 첫 번째 인자: 대시 입력 IA 가져오기
					ETriggerEvent::Triggered, // 두 번째 인자: 키가 눌렸을 때, 트리거
					this, // 세 번째 인자: 호출된 함수의 객체
					&ASpartaCharacter::StartSprint // 네 번째 인자 StartSprint 함수를 연결
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction, // 첫 번째 인자: 이동 입력 IA 가져오기
					ETriggerEvent::Completed, // 두 번째 인자: 키가 눌렸을 때, 트리거
					this, // 세 번째 인자: 호출된 함수의 객체
					&ASpartaCharacter::StopSprint // 네 번째 인자 StopSprint 함수를 연결
				);
			}
		}
	}

}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return; // Controller가 있는지 없는지 확인. Validation.

	// Move의 인자 value는 2D Vector로 받는다. Value Type을 따름.
	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X)) // float는 부동소수점이므로 0이 아닐 때 오차 보정해준다.
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X); //캐릭터 무브먼트 컴포넌트에서 구현해줌. 정면으로 X만큼 이동
	}
	if (!FMath::IsNearlyZero(MoveInput.Y)) // float는 부동소수점이므로 0이 아닐 때 오차 보정해준다.
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y); //캐릭터 무브먼트 컴포넌트에서 구현해줌. 오른쪽으로 Y만큼 이동
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	// Jump의 인자 value는 bool 
	if (value.Get<bool>())
	{
		Jump(); // Jump만 입력하면 끝. 점프 내부에도 Controller 확인 코드가 있다. 그리고 GetActorForwardVector 등을 가져오려면 컨트롤러가 있어야하기 때문에 move에서는 체크해줘야한다.
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping(); // StopJumping도 내부에 Controller 확인이 이미 구현되어있다.
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X); // Yaw 값은 좌우 회전이며, 캐릭터 클래스에 이미 구현되어 있음.
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

