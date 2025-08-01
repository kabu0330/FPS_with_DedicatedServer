// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_LocalPlayerSubsystem.h"

#include "UI/Portal/PortalManager.h"
#include "UI/Interface/PortalManagement.h"

void UDS_LocalPlayerSubsystem::InitializeTokens(const FDSAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManager)
{
	AuthenticationResult = AuthResult;
	PortalManagementInterface = PortalManager;
	
	float ExpiresTime = FCString::Atoi(*AuthenticationResult.ExpiresIn);
	TokenRefreshInterval = ExpiresTime * 0.8f; // 3600초 * 80% = 2880초마다 엑세스 토큰 갱신
	
	SetRefreshTokenTimer();
}

void UDS_LocalPlayerSubsystem::SetRefreshTokenTimer()
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

void UDS_LocalPlayerSubsystem::UpdateTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}

FDSAuthenticationResult UDS_LocalPlayerSubsystem::GetAuthenticationResult() const
{
	return AuthenticationResult;
}

FString UDS_LocalPlayerSubsystem::GetUsername() const
{
	return Username;
}

FString UDS_LocalPlayerSubsystem::GetEmail() const
{
	return Email;
}

void UDS_LocalPlayerSubsystem::SetUsername(const FString& Name)
{
	Username = Name;
}

void UDS_LocalPlayerSubsystem::SetEmail(const FString& EmailAddress)
{
	Email = EmailAddress;
}
