// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_MatchGameMode.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Game/DS_MatchGameState.h"
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

void ADS_MatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreateGameStatsManager();
}

void ADS_MatchGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
	if (MatchStatus != EMatchStatus::WaitingForPlayers)
	{
		ErrorMessage = TEXT("이미 게임이 진행 중입니다. 게임이 끝난 이후에 입장 가능합니다.");
		return;
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

void ADS_MatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	PlayerIsReadyForMatch(NewPlayer);
}

void ADS_MatchGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
}

void ADS_MatchGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
}

void ADS_MatchGameMode::HandleSeamlessTravelPlayer(AController*& Controller)
{
	Super::HandleSeamlessTravelPlayer(Controller);
}

void ADS_MatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerSession(Exiting);
	
	ReadyPlayerCount--;
	if (ReadyPlayerCount < 0) ReadyPlayerCount = 0;
	CheckAllPlayersIsReady(Exiting);
}

void ADS_MatchGameMode::PlayerIsReadyForMatch(AController* Controller)
{
	if (!Controller) return;
	if (MatchStatus == EMatchStatus::Match || MatchStatus == EMatchStatus::SeamlessTravelling) return;
	
	ReadyPlayerCount++;
	CheckAllPlayersIsReady(Controller);
}

void ADS_MatchGameMode::CheckAllPlayersIsReady(AController* Player)
{
	Super::CheckAllPlayersIsReady(Player);
	ADS_MatchGameState* GS = GetGameState<ADS_MatchGameState>();
	check(IsValid(GS));
	
	if (ReadyPlayerCount >= GetNumPlayers())
	{
		if (MatchStatus == EMatchStatus::WaitingForPlayers)
		{
			MatchStatus = EMatchStatus::PreMatch;
			StartCountdownTimer(PreMatchTimerHandle);
			GS->bIsMatchCountdownStarted = true; // 위젯에 다른 플레이어를 기다리는지 아닌지를 메시지를 띄우는 변수
		}
	}
	else
	{
		MatchStatus = EMatchStatus::WaitingForPlayers;
		StopCountdownTimer(PreMatchTimerHandle);
		GS->bIsMatchCountdownStarted = false;
	}
	
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_MatchGameMode CheckAllPlayersIsReady ReadyPlayerCount: %d, Total: %d"), ReadyPlayerCount, GetNumPlayers());
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

		ADS_MatchGameState* GS = GetGameState<ADS_MatchGameState>();
		check(IsValid(GS));
		GS->bIsMatchCountdownStarted = true;
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

