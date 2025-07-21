// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/PortalManagement.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "PortalManager.generated.h"



/**
 * 
 */
UCLASS()
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
	
	void SignIn(const FString& UserName, const FString& Password);
	void SignUp(const FString& UserName, const FString& Password, const FString& Email);
	void Confirm(const FString& ConfirmationCode);

	// IPortalManagement
	virtual void RefreshTokens(const FString& RefreshToken) override;

	UFUNCTION()
	void QuitGame();

	FDSSignUpResponse LastSignUpResponse;

private:
	FString LastUserName;
	
	void SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
	void Confirm_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
	void SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
	void RefreshTokens_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	

};
