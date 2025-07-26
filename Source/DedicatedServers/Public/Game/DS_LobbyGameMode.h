// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "DS_LobbyGameMode.generated.h"

class UDS_GameInstanceSubsystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_LobbyGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()
public:
	ADS_LobbyGameMode();
	void CheckAndStartLobbyCountdown();

protected:
	virtual void BeginPlay() override;
	
	/* Post Login
	 * 새로운 플레이어가 서버에 접속했을 때 서버에서 자동 호출
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/* Logout
	 * 플레이어가 게임 모드에서 나갔을 때
	 */
	virtual void Logout(AController* Exiting) override;
	
	/* InitSeamlessTravelPlayer
	 * 심리스 트래블로 넘어온 도착지 GameMode에 PlayerController들에게 호출
	 */
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	void CheckAndStopLobbyCountdown();
	
	UPROPERTY()
	ELobbyStatus LobbyStatus;

	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayers;

	/* TSoftObjectPtr : 에셋의 경로를 문자열로 저장하는 방식
	 * 메모리에 에셋을 로드하지 않고 필요할 때 로드하는 방식으로 최적화
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

};
