// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "Interfaces/IHttpRequest.h"
#include "DS_LobbyGameMode.generated.h"

class UDS_GameInstanceSubsystem;

/** 1. GameInstanceSubsystem->InitGameLift() 호출
 *  2. 로그인 시 가장 먼저 도착하는 레벨
 *  3. 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_LobbyGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()
public:
	ADS_LobbyGameMode();
	void CheckAllPlayersIsReady();

protected:
	// InitGameLift() 호출
	virtual void BeginPlay() override;
	
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	void CheckAndStopLobbyCountdown();
	void CheckAndStartLobbyCountdown();
	
	UPROPERTY()
	ELobbyStatus LobbyStatus;

	/** TSoftObjectPtr : 에셋의 경로를 문자열로 저장하는 방식
	 *  메모리에 에셋을 즉시 로드하지 않고 필요할 때 로드하는 방식으로 최적화
	 */
	UPROPERTY(EditdefaultsOnly)
	TSoftObjectPtr<UWorld> DestinationMap;
	
private:
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle LobbyCountdownTimer;
	
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	void TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage);
	void AddPlayerInfoToLobbyState(AController* Player) const;
	void RemovePlayerInfoFromLobbyState(AController* Player) const;

	void RecordUserInformation(APlayerController* NewPlayerController, const FString& Options);
	
	// 미사용
	void FindGameLiftAnywhereServerIP();
	void FindGameLiftAnywhereServerIP_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
