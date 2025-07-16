// Fill out your copyright notice in the Description page of Project Settings.


#include "DedicatedServers/Public/Game/DS_GameMode.h"
#include "DedicatedServers/DedicatedServers.h"

void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void ADS_GameMode::InitGameLift()
{
		#if WITH_GAMELIFT
    UE_LOG(Log_DedicatedServer, Log, TEXT("Calling InitGameLift..."));

    // Getting the module first.
    FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
    if (GameLiftSdkModule)
    {
        UE_LOG(Log_DedicatedServer, Log, TEXT("GameLiftServerSDK Load succeeded"));
    }
    //Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
    FServerParameters ServerParametersForAnywhere;
    SetServerParameters(ServerParametersForAnywhere);

    UE_LOG(Log_DedicatedServer, Log, TEXT("Initializing the GameLift Server..."));

    //InitSDK will establish a local connection with GameLift's agent to enable further communication.
    FGameLiftGenericOutcome InitSdkOutcome = GameLiftSdkModule->InitSDK(ServerParametersForAnywhere);
    if (InitSdkOutcome.IsSuccess())
    {
        UE_LOG(Log_DedicatedServer, SetColor, TEXT("%s"), COLOR_GREEN);
        UE_LOG(Log_DedicatedServer, Log, TEXT("GameLift InitSDK succeeded!"));
        UE_LOG(Log_DedicatedServer, SetColor, TEXT("%s"), COLOR_NONE);
    }
    else
    {
        UE_LOG(Log_DedicatedServer, SetColor, TEXT("%s"), COLOR_RED);
        UE_LOG(Log_DedicatedServer, Log, TEXT("ERROR: InitSDK failed : ("));
        FGameLiftError GameLiftError = InitSdkOutcome.GetError();
        UE_LOG(Log_DedicatedServer, Log, TEXT("ERROR: %s"), *GameLiftError.m_errorMessage);
        UE_LOG(Log_DedicatedServer, SetColor, TEXT("%s"), COLOR_NONE);
        return;
    }

    BindCallback(GameLiftSdkModule);

    ParesCommandLinePort();

    //Here, the game server tells Amazon GameLift Servers where to find game session log files.
    //At the end of a game session, Amazon GameLift Servers uploads everything in the specified 
    //location and stores it in the cloud for access later.
    TArray<FString> Logfiles;
    Logfiles.Add(TEXT("Log_DedicatedServer/Saved/Logs/Log_DedicatedServer.log"));
    ProcessParameters->logParameters = Logfiles;

    //The game server calls ProcessReady() to tell Amazon GameLift Servers it's ready to host game sessions.
    UE_LOG(Log_DedicatedServer, Log, TEXT("Calling Process Ready..."));
    FGameLiftGenericOutcome ProcessReadyOutcome = GameLiftSdkModule->ProcessReady(*ProcessParameters);

    if (ProcessReadyOutcome.IsSuccess())
    {
        UE_LOG(Log_DedicatedServer, SetColor, TEXT("%s"), COLOR_GREEN);
        UE_LOG(Log_DedicatedServer, Log, TEXT("Process Ready!"));
        UE_LOG(Log_DedicatedServer, SetColor, TEXT("%s"), COLOR_NONE);
    }
    else
    {
        UE_LOG(Log_DedicatedServer, SetColor, TEXT("%s"), COLOR_RED);
        UE_LOG(Log_DedicatedServer, Log, TEXT("ERROR: Process Ready Failed!"));
        FGameLiftError ProcessReadyError = ProcessReadyOutcome.GetError();
        UE_LOG(Log_DedicatedServer, Log, TEXT("ERROR: %s"), *ProcessReadyError.m_errorMessage);
        UE_LOG(Log_DedicatedServer, SetColor, TEXT("%s"), COLOR_NONE);
    }

    UE_LOG(Log_DedicatedServer, Log, TEXT("InitGameLift completed!"));
#endif
}

void ADS_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
    // bool bIsAnywhereActive = false;
    // if (FParse::Param(FCommandLine::Get(), TEXT("glAnywhere")))
    // {
    //     bIsAnywhereActive = true;
    //     UE_LOG(Log_DedicatedServer, Log, TEXT("glAnywhere found"));
    // }
    // else
    // {
    //     UE_LOG(Log_DedicatedServer, Log, TEXT("glAnywhere is not found"));
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

void ADS_GameMode::BindCallback(FGameLiftServerSDKModule* GameLiftSdkModule)
{
    ProcessParameters = MakeShared<FProcessParameters>();

    //When a game session is created, Amazon GameLift Servers sends an activation request to the game server and passes along the game session object containing game properties and other settings.
    //Here is where a game server should take action based on the game session object.
    //Once the game server is ready to receive incoming player connections, it should invoke GameLiftServerAPI.ActivateGameSession()
    ProcessParameters->OnStartGameSession.BindLambda([=](Aws::GameLift::Server::Model::GameSession InGameSession)
    {
        FString GameSessionId = FString(InGameSession.GetGameSessionId());
        UE_LOG(LogDedicatedServers, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
        GameLiftSdkModule->ActivateGameSession();
    });

    //OnProcessTerminate callback. Amazon GameLift Servers will invoke this callback before shutting down an instance hosting this game server.
    //It gives this game server a chance to save its state, communicate with services, etc., before being shut down.
    //In this case, we simply tell Amazon GameLift Servers we are indeed going to shutdown.
    ProcessParameters->OnTerminate.BindLambda([=]()
    {
        UE_LOG(LogDedicatedServers, Log, TEXT("Game Server Process is terminating"));
        GameLiftSdkModule->ProcessEnding();
    });

    //This is the HealthCheck callback.
    //Amazon GameLift Servers will invoke this callback every 60 seconds or so.
    //Here, a game server might want to check the health of dependencies and such.
    //Simply return true if healthy, false otherwise.
    //The game server has 60 seconds to respond with its health status. Amazon GameLift Servers will default to 'false' if the game server doesn't respond in time.
    //In this case, we're always healthy!
    ProcessParameters->OnHealthCheck.BindLambda([]()
    {
        UE_LOG(LogDedicatedServers, Log, TEXT("Performing Health Check"));
        return true;
    });
}

void ADS_GameMode::ParesCommandLinePort()
{
    //GameServer.exe -port=7777 LOG=server.mylog
    ProcessParameters->port = FURL::UrlConfig.DefaultPort;
    TArray<FString> CommandLineTokens;
    TArray<FString> CommandLineSwitches;

    FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches); // -port=7777

    for (const FString& SwitchStr : CommandLineSwitches)
    {
        FString Key;
        FString Value;

        if (SwitchStr.Split("=", &Key, &Value)) // port "=" 7777
        {
            if (Key.Equals("port", ESearchCase::IgnoreCase))
            {
                ProcessParameters->port = FCString::Atoi(*Value);
                return;
            }
        }
    }
}
