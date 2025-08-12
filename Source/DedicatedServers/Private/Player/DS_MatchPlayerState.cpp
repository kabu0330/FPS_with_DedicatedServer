// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_MatchPlayerState.h"

#include "Game/DS_GameInstanceSubsystem.h"
#include "Game/DS_MatchGameMode.h"
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


