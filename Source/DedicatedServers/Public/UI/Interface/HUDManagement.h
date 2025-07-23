// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDManagement.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHUDManagement : public UInterface
{
	GENERATED_BODY()
};

/** 로그인 화면 UI와 대시보드 UI 간 자유로운 전환을 구현하기 위해
 *  두 UI가 서로를 굳이 알 필요가 없도록 HUD 인터페이스를 통해서 처리
 */
class DEDICATEDSERVERS_API IHUDManagement
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnSignIn() = 0;
	virtual void OnSignOut() = 0;
};
