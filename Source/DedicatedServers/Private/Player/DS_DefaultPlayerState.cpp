// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_DefaultPlayerState.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Net/UnrealNetwork.h"

ADS_DefaultPlayerState::ADS_DefaultPlayerState()
{
}

void ADS_DefaultPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADS_DefaultPlayerState, DefaultUsername);
	DOREPLIFETIME(ADS_DefaultPlayerState, DefaultPlayerSessionId);
}

void ADS_DefaultPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	// Seamless Travel을 했을 때 데이터가 초기화되는 문제를 해결하기 위한 방법으로 시도
	if (ADS_DefaultPlayerState* NewPlayerState = Cast<ADS_DefaultPlayerState>(PlayerState); IsValid(NewPlayerState))
	{
		NewPlayerState->DefaultUsername = DefaultUsername;
		NewPlayerState->DefaultPlayerSessionId = DefaultPlayerSessionId;
		UE_LOG(LogTemp, Warning, TEXT("CopyProperties Username: %s"), *NewPlayerState->DefaultUsername);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not ADS_DefaultPlayerState"));
	}
}

void ADS_DefaultPlayerState::OverrideWith(APlayerState* PlayerState)
{
	Super::OverrideWith(PlayerState);
	if (ADS_DefaultPlayerState* NewPlayerState = Cast<ADS_DefaultPlayerState>(PlayerState); IsValid(NewPlayerState))
	{
		DefaultUsername = NewPlayerState->DefaultUsername;
		DefaultPlayerSessionId = NewPlayerState->DefaultPlayerSessionId;
		UE_LOG(LogTemp, Warning, TEXT("OverrideWith Username: %s"), *DefaultUsername);
	}
}

FString ADS_DefaultPlayerState::GetUsername() const
{
	return DefaultUsername;
}

FString ADS_DefaultPlayerState::GetPlayerSessionId() const
{
	return DefaultPlayerSessionId;
}

void ADS_DefaultPlayerState::SetUsername(const FString& Username)
{
	if (HasAuthority())
	{
		DefaultUsername = Username;
		OnRep_DisplayUsername();
	}
}

void ADS_DefaultPlayerState::SetPlayerSessionId(const FString& SessionId)
{
	if (HasAuthority())
	{
		DefaultPlayerSessionId = SessionId;
		OnRep_DisplayPlayerSessionId();
	}
}

void ADS_DefaultPlayerState::OnRep_DisplayUsername()
{
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_DefaultPlayerState SetUsername: %s"), *DefaultUsername);
}

void ADS_DefaultPlayerState::OnRep_DisplayPlayerSessionId()
{
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_DefaultPlayerState SetPlayerSessionId: %s"), *DefaultPlayerSessionId);
}

void ADS_DefaultPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}
