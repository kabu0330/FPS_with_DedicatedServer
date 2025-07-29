// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Types/DSTypes.h"
#include "DS_GameModeBase.generated.h"

/** 1. 게임 내 각종 타이머 기능을 책임진다.
 *  서버만이 시간을 계산하고 그 결과는 PlayerController가 ping-pong 지연시간 평균을 계산해서 추가로 감산한다.
 *  2. 서버 트래블 구현 함수 제공
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameModeBase : public AGameMode
{
	GENERATED_BODY()

protected:
	void StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);
	void StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);

	void UpdateCountdownTimer(const FCountdownTimerHandle& CountdownTimerHandle);

	virtual void OnCountdownTimerFinished(ECountdownTimerType Type);

	// Server Travel
	void TrySeamlessTravel(TSoftObjectPtr<UWorld> DestinationMap);

	void RemovePlayerSession(AController* Exiting);
};
