// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "DS_MatchGameMode.generated.h"

class UGameStatsManager;

/** 1. 실제 컨텐츠 레벨의 부모 클래스
 *  2. 전투 관련 타이머 + 심리스 서버 트래블(로비 복귀)
 *  3. 게임 종료 후 DB 기록 함수 호출
 */ 
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

public:
	ADS_MatchGameMode();
	virtual void CheckAllPlayersIsReady(AController* Player) override;

	void PlayerIsReadyForMatch(AController* Controller);

	UGameStatsManager* GetGameStatsManager();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;

protected:
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimerHandle;

	UPROPERTY(EditdefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	virtual void HandleSeamlessTravelPlayer(AController*& Controller) override;

	
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	
	virtual void OnMatchEnded();

	UFUNCTION()
	virtual void OnGameStatsUpdated();
	
	void UpdateLeaderboard(const TArray<FString>& LeaderboardNames);

private:
	UPROPERTY()
	EMatchStatus MatchStatus;
	
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;

	int32 ReadyPlayerCount = 0;
	
	void SetClientInputEnabled(bool bEnabled);
	void EndMatchForPlayerStats();
	void CreateGameStatsManager();
	
};
