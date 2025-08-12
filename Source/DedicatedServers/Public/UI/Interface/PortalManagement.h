// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PortalManagement.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPortalManagement : public UInterface
{
	GENERATED_BODY()
};

/** LocalPlayerSubsystem과 PortalManager 간 결합도를 낮추기 위해 인터페이스 사용
 *  LocalPlayerSubsystem은 사용자 정보를 저장하고 싶고, 그 정보는 PortalManager가 HTTP Response를 처리하는 과정에 개입해 가져온다.
 */
class DEDICATEDSERVERS_API IPortalManagement
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RefreshTokens(const FString& RefreshToken) = 0;
};
