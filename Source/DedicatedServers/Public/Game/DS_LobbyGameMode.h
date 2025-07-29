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
	void CheckAndStartLobbyCountdown();

protected:
	// InitGameLift() 호출
	virtual void BeginPlay() override;

	// 새로운 플레이어가 서버에 접속했을 때 서버에서 자동 호출
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 플레이어가 게임 모드에서 나갔을 때
	virtual void Logout(AController* Exiting) override;
	
	// 심리스 트래블로 넘어온 도착지 GameMode에 PlayerController들에게 호출
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	void CheckAndStopLobbyCountdown();

	/** 클라이언트가 접속을 시도할 때 가장 먼저 호출되는 함수
	 *  PlayerController와 같은 주요 액터가 생성되기 전에 호출되므로 접속을 허용할지 말지를 가장 먼저 결정하는 단계
	 */
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;
	
	UPROPERTY()
	ELobbyStatus LobbyStatus;

	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayers;

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

	// 미사용
	void FindGameLiftAnywhereServerIP();
	void FindGameLiftAnywhereServerIP_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
