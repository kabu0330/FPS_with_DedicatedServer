// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_MatchGameMode.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "Player/DS_MatchPlayerState.h"
#include "Player/DS_PlayerController.h"
#include "UI/GameStats/GameStatsManager.h"

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

void ADS_MatchGameMode::CreateGameStatsManager()
{
	check(GameStatsManagerClass);
	if (IsValid(GameStatsManager)) return;
	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	
	if (!IsValid(GameStatsManager)) return;
	GameStatsManager->OnUpdatedGameStatsSucceeded.AddDynamic(this, &ADS_MatchGameMode::OnGameStatsUpdated);
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_MatchGameMode BeginPlay GameStatsManager is created"));
}

UGameStatsManager* ADS_MatchGameMode::GetGameStatsManager()
{
	if (!IsValid(GameStatsManager))
	{
		CreateGameStatsManager();
	}
	return GameStatsManager;
}

void ADS_MatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreateGameStatsManager();
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
		
		OnMatchEnded();
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
		ADS_PlayerController* DSPlayerController = Cast<ADS_PlayerController>(It->Get());
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_SetInputEnabled(bEnabled);
		}
	}
}

void ADS_MatchGameMode::UpdateLeaderboard(const TArray<FString>& LeaderboardNames)
{
	if (IsValid(GameStatsManager))
	{
		GameStatsManager->UpdateLeaderboard(LeaderboardNames);
	}
}

void ADS_MatchGameMode::EndMatchForPlayerStats()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ADS_PlayerController* DSPlayerController = Cast<ADS_PlayerController>(It->Get()); IsValid(DSPlayerController))
		{
			if (ADS_MatchPlayerState* MatchPlayerState = Cast<ADS_MatchPlayerState>(DSPlayerController->PlayerState); IsValid(MatchPlayerState))
			{
				MatchPlayerState->OnMatchEnded();
			}
		}
	}
}

void ADS_MatchGameMode::OnMatchEnded()
{
	EndMatchForPlayerStats();
}

void ADS_MatchGameMode::OnGameStatsUpdated()
{
	// Content Moudule: ShooterGameMode override
	// UpdateLeaderboard()
}

