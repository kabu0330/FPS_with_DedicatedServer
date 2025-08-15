// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Interface/PortalManagement.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "PortalManager.generated.h"



/** 로그인, 회원가입, 인증, 로그아웃 HTTP 통신 기능 구현
 * 
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UPortalManager : public UHTTPRequestManager, public IPortalManagement
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessgae SignUpStatusMessageDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessgae ConfirmStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessgae SignInStatusMessageDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnSignUpSucceededDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnConfirmSucceededDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnSignInSucceededDelegate;
	
	void SignIn(const FString& Username, const FString& Password);
	void SignUp(const FString& Username, const FString& Password, const FString& Email);
	void Confirm(const FString& ConfirmationCode);
	void SignOut(const FString& AccessToken);

	// IPortalManagement
	virtual void RefreshTokens(const FString& RefreshToken) override;

	UFUNCTION(BlueprintCallable)
	void QuitGame();

	FDSSignUpResponse LastSignUpResponse;

private:
	FString LastUserName;
	
	void SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
	void SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
	void Confirm_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
	void RefreshTokens_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
	void SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	

};




