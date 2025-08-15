// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_DefaultPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "DS_MatchPlayerState.generated.h"

struct FDSRecordMatchStatsInput;
class UGameStatsManager;
/** 1. 게임이 끝나고 플레이 기록을 DB에 저장하는 기능을 제공하는 부모 클래스
 *  2. 컨텐츠 모듈에서 해당 클래스를 상속하여 플레이 데이터를 기록해야 함
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchPlayerState : public ADS_DefaultPlayerState
{
	GENERATED_BODY()
public:
	virtual void OnMatchEnded();
	void PlayerIsReadyForLobby(bool IsReady);

	void PlayerIsReadyForMatch();

protected:
	virtual void BeginPlay() override;

	void RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput) const;

private:
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;
	
	void TakeGameStatsManager();

	UFUNCTION(Server, Reliable)
	void Server_PlayerIsReadyForLobby(bool IsReady);


};
