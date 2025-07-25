// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameInstanceSubsystem.h"

#include "DedicatedServers/DedicatedServers.h"

UDS_GameInstanceSubsystem::UDS_GameInstanceSubsystem()
{
	bGameLiftInitialized = false;
}

void UDS_GameInstanceSubsystem::InitGameLift(const FServerParameters& ServerParameters)
{
	if (bGameLiftInitialized) return;

#if WITH_GAMELIFT
    UE_LOG(LogDedicatedServers, Log, TEXT("Initializing the GameLift Servers"));

    // Getting the module first.
    FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
    if (GameLiftSdkModule)
    {
        UE_LOG(LogDedicatedServers, Log, TEXT("GameLiftServerSDK Load succeeded"));
    }
    //Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
    //FServerParameters ServerParametersForAnywhere;
    //SetServerParameters(ServerParametersForAnywhere);

    UE_LOG(LogDedicatedServers, Log, TEXT("Initializing the GameLift Server..."));

    //InitSDK will establish a local connection with GameLift's agent to enable further communication.
    FGameLiftGenericOutcome InitSdkOutcome = GameLiftSdkModule->InitSDK(ServerParameters);
    if (InitSdkOutcome.IsSuccess())
    {
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_GREEN);
        UE_LOG(LogDedicatedServers, Log, TEXT("GameLift InitSDK succeeded!"));
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
    }
    else
    {
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_RED);
        UE_LOG(LogDedicatedServers, Log, TEXT("ERROR: InitSDK failed : ("));
        FGameLiftError GameLiftError = InitSdkOutcome.GetError();
        UE_LOG(LogDedicatedServers, Log, TEXT("ERROR: %s"), *GameLiftError.m_errorMessage);
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
        return;
    }

    BindCallback(GameLiftSdkModule);

    ParesCommandLinePort();

    //Here, the game server tells Amazon GameLift Servers where to find game session log files.
    //At the end of a game session, Amazon GameLift Servers uploads everything in the specified 
    //location and stores it in the cloud for access later.
    TArray<FString> Logfiles;
    Logfiles.Add(TEXT("LogDedicatedServer/Saved/Logs/LogDedicatedServer.log"));
    ProcessParameters->logParameters = Logfiles;

    //The game server calls ProcessReady() to tell Amazon GameLift Servers it's ready to host game sessions.
    UE_LOG(LogDedicatedServers, Log, TEXT("Calling Process Ready..."));
    FGameLiftGenericOutcome ProcessReadyOutcome = GameLiftSdkModule->ProcessReady(*ProcessParameters);

    if (ProcessReadyOutcome.IsSuccess())
    {
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_GREEN);
        UE_LOG(LogDedicatedServers, Log, TEXT("Process Ready!"));
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
    }
    else
    {
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_RED);
        UE_LOG(LogDedicatedServers, Log, TEXT("ERROR: Process Ready Failed!"));
        FGameLiftError ProcessReadyError = ProcessReadyOutcome.GetError();
        UE_LOG(LogDedicatedServers, Log, TEXT("ERROR: %s"), *ProcessReadyError.m_errorMessage);
        UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
    }

    UE_LOG(LogDedicatedServers, Log, TEXT("InitGameLift completed!"));
#endif

    bGameLiftInitialized = true;
}

void UDS_GameInstanceSubsystem::BindCallback(FGameLiftServerSDKModule* GameLiftSdkModule)
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

void UDS_GameInstanceSubsystem::ParesCommandLinePort()
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
            if (Key.Equals("port", ESearchCase::IgnoreCase) && Value.IsNumeric())
            {
                //ProcessParameters->port = FCString::Atoi(*Value);
                LexFromString(ProcessParameters->port, *Value);
                return;
            }
        }
    }
}
