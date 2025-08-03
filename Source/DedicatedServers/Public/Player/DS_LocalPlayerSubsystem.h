// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "DS_LocalPlayerSubsystem.generated.h"

class IPortalManagement;

/** 1. 사용자 정보를 PortalManager로부터 가져온다.
 *  2. 주기적으로 Token을 갱신한다.
 */
UCLASS()
class DEDICATEDSERVERS_API UDS_LocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	void InitializeTokens(const FDSAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManager);
	void SetRefreshTokenTimer();
	void UpdateTokens(const FString& AccessToken, const FString& IdToken);
	FDSAuthenticationResult GetAuthenticationResult() const;
	
	FString GetUsername() const;
	FString GetEmail() const;
	FString GetPassword() const;
	void SetUsername(const FString& Name);
	void SetEmail(const FString& EmailAddress);
	void SetPassword(const FString& InputPassword);
	
private:
	UPROPERTY()
	FDSAuthenticationResult AuthenticationResult;

	UPROPERTY()
	TScriptInterface<IPortalManagement> PortalManagementInterface;

	float TokenRefreshInterval = 1.0f;
	FTimerHandle TokenRefreshTimer;

	FString Username;
	FString Email;
	FString Password{};
};
