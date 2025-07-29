// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_MatchGameMode.h"

#include "Player/DSPlayerController.h"

ADS_MatchGameMode::ADS_MatchGameMode()
{
	bUseSeamlessTravel = true;
	MatchStatus = EMatchStatus::WaitingForPlayers;
	PreMatchTimerHandle.Type = ECountdownTimerType::PreMatch;
	MatchTimerHandle.Type = ECountdownTimerType::Match;
	PostMatchTimerHandle.Type = ECountdownTimerType::PostMatch;
	
}

void ADS_MatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	/** 처음에는 PreMatchTimer를 전달하고 x초 후에 OnCountdownTimerFinished 함수로 넘어오면
	 ** Match -> PostMatch 타이머가 순차적으로 돌아간다.
	 */
	
	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountdownTimer(PreMatchTimerHandle);
	}
}

void ADS_MatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerSession(Exiting);
}

void ADS_MatchGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountdownTimer(PreMatchTimerHandle);
	}
}

void ADS_MatchGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);

	if (Type == ECountdownTimerType::PreMatch)
	{
		StopCountdownTimer(PreMatchTimerHandle);
		MatchStatus = EMatchStatus::Match;
		StartCountdownTimer(MatchTimerHandle);
		SetClientInputEnabled(true);
	}
	if (Type == ECountdownTimerType::Match)
	{
		StopCountdownTimer(MatchTimerHandle);
		MatchStatus = EMatchStatus::PostMatch;
		StartCountdownTimer(PostMatchTimerHandle);
		SetClientInputEnabled(false);
	}
	if (Type == ECountdownTimerType::PostMatch)
	{
		StopCountdownTimer(PostMatchTimerHandle);
		MatchStatus = EMatchStatus::SeamlessTravelling;
		// SeamlessTravelling
		TrySeamlessTravel(LobbyMap);
	}
}

void ADS_MatchGameMode::SetClientInputEnabled(bool bEnabled)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(It->Get());
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_SetInputEnabled(bEnabled);
		}
	}
}
