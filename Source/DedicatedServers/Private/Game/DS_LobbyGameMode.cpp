// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_LobbyGameMode.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
    bUseSeamlessTravel = true;
    
    LobbyStatus = ELobbyStatus::WaitingForPlayers;
    MinPlayers = 1;
    LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown;
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    CheckAndStartLobbyCountdown();
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
    Super::InitSeamlessTravelPlayer(NewController);
    CheckAndStartLobbyCountdown();
}


void ADS_LobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    CheckAndStopLobbyCountdown();
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

void ADS_LobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
    Super::OnCountdownTimerFinished(Type);

    if (Type == ECountdownTimerType::LobbyCountdown)
    {
        LobbyStatus = ELobbyStatus::SeamlessTravelling;
        const FString MapName = DestinationMap.ToSoftObjectPath().GetAssetName();
        /** 서버 트래블은 패키징에서만 정상 동작하므로 테스트 환경에서 확인할 수 있도록 코드를 분리
         * GIsEditor 매크로를 통해서 에디터 모드인지 아닌지를 구분할 수 있다.
         */
        if (GIsEditor)
        {
            UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
        }
        else
        {
            GetWorld()->ServerTravel(MapName);
        }
    }
}

void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
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

