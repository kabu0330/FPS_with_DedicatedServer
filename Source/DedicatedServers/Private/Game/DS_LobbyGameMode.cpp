// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_LobbyGameMode.h"

#include "HttpModule.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Game/DS_GameState.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyState.h"
#include "Player/DS_DefaultPlayerState.h"
#include "Player/DS_PlayerController.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
    bUseSeamlessTravel = true;
    
    LobbyStatus = ELobbyStatus::WaitingForPlayers;
    MinPlayers = 1;
    LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown;
    
    PlayerStateClass = ADS_DefaultPlayerState::StaticClass();
}

void ADS_LobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
    FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
    const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

    UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode PreLogin for %s"), *Username);
    TryAcceptPlayerSession(PlayerSessionId, Username, ErrorMessage);
}

FString ADS_LobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
    const FString& Options, const FString& Portal)
{
    FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
    
    RecordUserInformation(NewPlayerController, Options);
    
    return InitializedString;
}

void ADS_LobbyGameMode::RecordUserInformation(APlayerController* NewPlayerController, const FString& Options)
{
    const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
    const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));
    
    ADS_PlayerController* PlayerController = Cast<ADS_PlayerController>(NewPlayerController);
    if (!IsValid(PlayerController))
    {
        UE_LOG(LogDedicatedServers, Error, TEXT("ADS_LobbyGameMode InitNewPlayer: PlayerController is not valid"));
    }
    
    ADS_DefaultPlayerState* PlayerState = NewPlayerController->GetPlayerState<ADS_DefaultPlayerState>(); 
    if (IsValid(PlayerState))
    {
        if (!PlayerSessionId.IsEmpty())
        {
            PlayerState->SetPlayerSessionId(PlayerSessionId);
        }
        if (!Username.IsEmpty())
        {
            PlayerState->SetUsername(Username);
        }
        UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode InitNewPlayer for %s, %s"), *PlayerState->GetUsername(), *NewPlayerController->GetName());
    }
    else
    {
        UE_LOG(LogDedicatedServers, Error, TEXT("ADS_LobbyGameMode InitNewPlayer: PlayerState is not valid"));
    }
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    CheckAndStartLobbyCountdown();

    FString Username{};
    ADS_DefaultPlayerState* PlayerState = NewPlayer->GetPlayerState<ADS_DefaultPlayerState>();
    if (IsValid(PlayerState))
    {
        Username = PlayerState->GetUsername();    
    }
    
    UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode PostLogin for %s, %s"), *Username, *NewPlayer->GetName());

    AddPlayerInfoToLobbyState(NewPlayer);
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
    Super::InitSeamlessTravelPlayer(NewController);
    CheckAndStartLobbyCountdown();

    AddPlayerInfoToLobbyState(NewController);
    
    UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode InitSeamlessTravelPlayer for %s"), *NewController->GetName());
}

void ADS_LobbyGameMode::AddPlayerInfoToLobbyState(AController* Player) const
{
    if (LobbyStatus == ELobbyStatus::SeamlessTravelling) return;

    ADS_PlayerController* PlayerController = Cast<ADS_PlayerController>(Player);
    ADS_GameState* DSGameState = GetGameState<ADS_GameState>();
    ADS_DefaultPlayerState* PlayerState = PlayerController->GetPlayerState<ADS_DefaultPlayerState>();
    if (IsValid(DSGameState) && IsValid(DSGameState->LobbyState) && IsValid(PlayerController) && IsValid(PlayerState))
    {
        FLobbyPlayerInfo PlayerInfo(PlayerState->GetUsername());
        DSGameState->LobbyState->AddPlayerInfo(PlayerInfo);
    }
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    CheckAndStopLobbyCountdown();
    if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
    {
        RemovePlayerInfoFromLobbyState(Exiting);
    }
    
    RemovePlayerSession(Exiting);
    
    UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode Logout for %s"), *Exiting->GetName());
}

void ADS_LobbyGameMode::RemovePlayerInfoFromLobbyState(AController* Player) const
{
    ADS_PlayerController* PlayerController = Cast<ADS_PlayerController>(Player);
    ADS_GameState* DSGameState = GetGameState<ADS_GameState>();
    ADS_DefaultPlayerState* PlayerState = PlayerController->GetPlayerState<ADS_DefaultPlayerState>();
    if (IsValid(DSGameState) && IsValid(DSGameState->LobbyState) && IsValid(PlayerController) && IsValid(PlayerState))
    {
        DSGameState->LobbyState->RemovePlayerInfo(PlayerState->GetUsername());
    }
}

void ADS_LobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
    Super::OnCountdownTimerFinished(Type);

    if (Type == ECountdownTimerType::LobbyCountdown)
    {
        StopCountdownTimer(LobbyCountdownTimer);
        LobbyStatus = ELobbyStatus::SeamlessTravelling;
        TrySeamlessTravel(DestinationMap);
    }
}

void ADS_LobbyGameMode::CheckAndStartLobbyCountdown()
{
    if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
    {
        LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
        StartCountdownTimer(LobbyCountdownTimer);
    }
}

void ADS_LobbyGameMode::CheckAndStopLobbyCountdown()
{
    if (GetNumPlayers() - 1 < MinPlayers && LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
    {
        LobbyStatus = ELobbyStatus::WaitingForPlayers;
        StopCountdownTimer(LobbyCountdownTimer);
    }
}

void ADS_LobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username,
                                               FString& OutErrorMessage)
{
    if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
    {
        OutErrorMessage = TEXT("Invalid player session id or username");
        return;
    }
#if WITH_GAMELIFT
    Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionsRequest;
    DescribePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));

    const auto& DescribePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionsRequest);
    if (!DescribePlayerSessionsOutcome.IsSuccess())
    {
        OutErrorMessage = TEXT("DescribePlayerSessions failed");
        return;
    }

    const auto& DescribePlayerSessionsResult = DescribePlayerSessionsOutcome.GetResult();
    // 플레이어 세션 수와 PlayerSessions 배열을 반환한다.
    int32 Count = 0;
    const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(Count);
    if (PlayerSessions == nullptr || Count == 0)
    {
        OutErrorMessage = TEXT("GetPlayerSessions failed");
        return;
    }

    for (int32 i = 0; i < Count; ++i)
    {
        const Aws::GameLift::Server::Model::PlayerSession& PlayerSession =  PlayerSessions[i];
        if (!Username.Equals(PlayerSession.GetPlayerId())) continue;
        if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
        {
            OutErrorMessage = FString::Printf(TEXT("Player session %s is not reserved"), *PlayerSessionId);
            return;
        }

        const auto& AcceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
        OutErrorMessage = AcceptPlayerSessionOutcome.IsSuccess() ? TEXT("") :FString::Printf( TEXT("AcceptPlayerSession failed"));
    } // OutErrorMessage가 빈 문자열이라면 성공적으로 플레이어 세션을 수락했음을 의미한다.
#endif
    
}

void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode BeginPlay"));
	InitGameLift();
}

void ADS_LobbyGameMode::InitGameLift()
{
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>(); IsValid(DSGameInstanceSubsystem))
		{
			FServerParameters ServerParameters;
		    SetServerParameters(ServerParameters);
		    DSGameInstanceSubsystem->InitGameLift(ServerParameters);
		    UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode InitGameLift() end..."));
		}
	}
}

void ADS_LobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	 // bool bIsAnywhereActive = false;
    // if (FParse::Param(FCommandLine::Get(), TEXT("glAnywhere")))
    // {
    //     bIsAnywhereActive = true;
    //     UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhere found"));
    // }
    // else
    // {
    //     UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhere is not found"));
    // }
    
    // if (bIsAnywhereActive)
    // {
        UE_LOG(LogDedicatedServers, Log, TEXT("Configuring server parameters for Anywhere..."));
        
        // If GameLift Anywhere is enabled, parse command line arguments and pass them in the ServerParameters object.
        FString glAnywhereWebSocketUrl = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereWebSocketUrl="), glAnywhereWebSocketUrl))
        {
            OutServerParameters.m_webSocketUrl = TCHAR_TO_UTF8(*glAnywhereWebSocketUrl);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereWebSocketUrl succeeded!"));
        }

        FString glAnywhereFleetId = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereFleetId="), glAnywhereFleetId))
        {
            OutServerParameters.m_fleetId = TCHAR_TO_UTF8(*glAnywhereFleetId);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereFleetId succeeded!"));
        }

        FString glAnywhereProcessId = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereProcessId="), glAnywhereProcessId))
        {
            OutServerParameters.m_processId = TCHAR_TO_UTF8(*glAnywhereProcessId);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereProcessId succeeded!"));
        }
        else
        {
            // If no ProcessId is passed as a command line argument, generate a randomized unique string.
            FString TimeString = FString::FromInt(std::time(nullptr));
            FString ProcessId = "ProcessId_" + TimeString;
            OutServerParameters.m_processId = TCHAR_TO_UTF8(*ProcessId);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereProcessId succeeded!"));
        }

        FString glAnywhereHostId = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereHostId="), glAnywhereHostId))
        {
            OutServerParameters.m_hostId = TCHAR_TO_UTF8(*glAnywhereHostId);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereHostId succeeded!"));
        }

        FString glAnywhereAuthToken = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereAuthToken="), glAnywhereAuthToken))
        {
            OutServerParameters.m_authToken = TCHAR_TO_UTF8(*glAnywhereAuthToken);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereAuthToken succeeded!"));
        }

        FString glAnywhereAwsRegion = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereAwsRegion="), glAnywhereAwsRegion))
        {
            OutServerParameters.m_awsRegion = TCHAR_TO_UTF8(*glAnywhereAwsRegion);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereAwsRegion succeeded!"));
        }

        FString glAnywhereAccessKey = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereAccessKey="), glAnywhereAccessKey))
        {
            OutServerParameters.m_accessKey = TCHAR_TO_UTF8(*glAnywhereAccessKey);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereAccessKey succeeded!"));
        }

        FString glAnywhereSecretKey = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereSecretKey="), glAnywhereSecretKey))
        {
            OutServerParameters.m_secretKey = TCHAR_TO_UTF8(*glAnywhereSecretKey);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereSecretKey succeeded!"));
        }

        FString glAnywhereSessionToken = "";
        if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereSessionToken="), glAnywhereSessionToken))
        {
            OutServerParameters.m_sessionToken = TCHAR_TO_UTF8(*glAnywhereSessionToken);
            UE_LOG(LogDedicatedServers, Log, TEXT("glAnywhereSessionToken succeeded!"));
        }
   
    //}
    
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_YELLOW);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> WebSocket URL: %s"), *OutServerParameters.m_webSocketUrl);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> Fleet ID: %s"), *OutServerParameters.m_fleetId);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> Process ID: %s"), *OutServerParameters.m_processId);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> Host ID (Compute Name): %s"), *OutServerParameters.m_hostId);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> Auth Token: %s"), *OutServerParameters.m_authToken);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> Aws Region: %s"), *OutServerParameters.m_awsRegion);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> Access Key: %s"), *OutServerParameters.m_accessKey);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> Secret Key: %s"), *OutServerParameters.m_secretKey);
        UE_LOG(LogDedicatedServers, Log, TEXT(">>>> Session Token: %s"), *OutServerParameters.m_sessionToken);
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
    

}

void ADS_LobbyGameMode::FindGameLiftAnywhereServerIP()
{
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, & ADS_LobbyGameMode::FindGameLiftAnywhereServerIP_Response);

    Request->SetURL(TEXT("https://checkip.amazonaws.com"));
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));
	
    Request->ProcessRequest();
}

void ADS_LobbyGameMode::FindGameLiftAnywhereServerIP_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                                              bool bWasSuccessful)
{
    if (!bWasSuccessful)
    {
        UE_LOG(LogDedicatedServers, Warning, TEXT("FindGameLiftAnywhereServerIP_Response is Failed..."));
        return;
    }
	
    FString AnywhereIP = TEXT("");
    if (Response.IsValid())
    {
        AnywhereIP = Response->GetContentAsString().TrimStartAndEnd();
        UE_LOG(LogDedicatedServers, Warning, TEXT("AnywhereIP : %s"), *AnywhereIP);
    }
    else
    {
        UE_LOG(LogDedicatedServers, Error, TEXT("FindGameLiftAnywhereServerIP_Response is Failed..."));
    }
}
