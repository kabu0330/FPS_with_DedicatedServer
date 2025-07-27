// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "DS_MatchGameMode.generated.h"

/** 1. 실제 컨텐츠 레벨의 부모 클래스
 *  2. 전투 관련 타이머 + 심리스 서버 트래블(로비 복귀)
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

public:
	ADS_MatchGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimerHandle;

	UPROPERTY(EditdefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;

	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;

	void SetClientInputEnabled(bool bEnabled);
	
private:
	UPROPERTY()
	EMatchStatus MatchStatus;
	
};
