// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSLocalPlayerSubsystem.h"

#include "UI/Portal/PortalManager.h"
#include "UI/Interface/PortalManagement.h"

void UDSLocalPlayerSubsystem::InitializeTokens(const FDSAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManager)
{
	AuthenticationResult = AuthResult;
	PortalManagementInterface = PortalManager;
	
	float ExpiresTime = FCString::Atoi(*AuthenticationResult.ExpiresIn);
	TokenRefreshInterval = ExpiresTime * 0.8f; // 3600초 * 80% = 2880초마다 엑세스 토큰 갱신
	
	SetRefreshTokenTimer();
}

void UDSLocalPlayerSubsystem::SetRefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(PortalManagementInterface.GetObject()))
	{
		FTimerDelegate RefreshTokenDelegate;
		RefreshTokenDelegate.BindLambda([this]()
		{
			PortalManagementInterface->RefreshTokens(AuthenticationResult.RefreshToken);
		});
		
		World->GetTimerManager().SetTimer(TokenRefreshTimer, RefreshTokenDelegate, TokenRefreshInterval, false);
	}
}

void UDSLocalPlayerSubsystem::UpdateTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}

FDSAuthenticationResult UDSLocalPlayerSubsystem::GetAuthenticationResult() const
{
	return AuthenticationResult;
}

FString UDSLocalPlayerSubsystem::GetUsername() const
{
	return Username;
}

FString UDSLocalPlayerSubsystem::GetEmail() const
{
	return Email;
}

void UDSLocalPlayerSubsystem::SetUsername(const FString& Name)
{
	Username = Name;
}

void UDSLocalPlayerSubsystem::SetEmail(const FString& EmailAddress)
{
	Email = EmailAddress;
}
