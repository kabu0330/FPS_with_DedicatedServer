// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "DS_MatchGameMode.generated.h"

/**
 * 
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

	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	
private:
	UPROPERTY()
	EMatchStatus MatchStatus;
	
};
