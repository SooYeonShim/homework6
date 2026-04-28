// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"

ASpartaPlayerController::ASpartaPlayerController(): 
	InputMappingContext(nullptr), // 아무것도 할당하지 않는 것보다는 nullptr을 할당해주는 것이 좋다.
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr)
{

}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어를 가져오기 -> 향상된 입력 관리하는 서브시스템 획득하기 -> IMC 객체 확인하고 서브시스템에 IMC 추가하며 우선순위를 최우선으로 두고 활성화시키기
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // GetLocalPlayer는 플레이어의 로컬 객체를 가져옴
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // 입력 시스템을 관리하는 서브시스템. IMC를 추가하거나 삭제하는 역할을 한다.
		{
			if (InputMappingContext) // 인풋 매핑 컨텍스트가 존재하면
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); // InputMappingContext로 활성화함. 0은 우선순위를 뜻함.
			}
		}
	}
}
