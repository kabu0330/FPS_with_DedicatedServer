// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_MatchPlayerState.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Game/DS_GameState.h"
#include "Game/DS_MatchGameMode.h"
#include "Net/UnrealNetwork.h"
#include "UI/GameStats/GameStatsManager.h"

void ADS_MatchPlayerState::OnMatchEnded()
{
	// Content Module: MatchPlayerState override
}


void ADS_MatchPlayerState::BeginPlay()
{
	Super::BeginPlay();

	TakeGameStatsManager();
}

void ADS_MatchPlayerState::TakeGameStatsManager()
{
	if (HasAuthority())
	{
		ADS_MatchGameMode* GameMode = Cast<ADS_MatchGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(GameMode))
		{
			GameStatsManager = GameMode->GetGameStatsManager();
		}
	}
}

void ADS_MatchPlayerState::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput) const
{
	check(IsValid(GameStatsManager));
	GameStatsManager->RecordMatchStats(RecordMatchStatsInput);
}

void ADS_MatchPlayerState::PlayerIsReadyForLobby(bool IsReady)
{
	Server_PlayerIsReadyForLobby(IsReady);
}

void ADS_MatchPlayerState::PlayerIsReadyForMatch()
{
	if (ADS_MatchGameMode* GameMode = GetWorld()->GetAuthGameMode<ADS_MatchGameMode>(); IsValid(GameMode))
	{
		GameMode->PlayerIsReadyForMatch(GetPlayerController());
	}
}

void ADS_MatchPlayerState::Server_PlayerIsReadyForLobby_Implementation(bool IsReady)
{
	if (ADS_GameState* GameState = GetWorld()->GetGameState<ADS_GameState>(); IsValid(GameState))
	{
		GameState->GetPlayerList().SetPlayerReady(GetUsername(), IsReady);
	}
	if (ADS_GameModeBase* GameMode = GetWorld()->GetAuthGameMode<ADS_GameModeBase>(); IsValid(GameMode))
	{
		GameMode->CheckAllPlayersIsReady(GetPlayerController());
	}
}
