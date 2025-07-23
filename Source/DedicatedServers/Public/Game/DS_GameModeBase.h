// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Types/DSTypes.h"
#include "DS_GameModeBase.generated.h"

/** 게임 내 각종 타이머 기능만 책임진다.
 *  서버만이 시간을 계산하고 그 결과는 PlayerController가 ping-pong 지연시간 평균을 계산해서 추가로 감산한다.
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameModeBase : public AGameMode
{
	GENERATED_BODY()

protected:
	void StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);
	void StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);

	virtual void OnCountdownTimerFinished(ECountdownTimerType Type);
};
