// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass(); // 코드로 디폴트 폰 클래스 적용. StaticClass는 객체를 생성하지 않고 메타데이터로 선언해준다. UCLASS 타입을 반환
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
}