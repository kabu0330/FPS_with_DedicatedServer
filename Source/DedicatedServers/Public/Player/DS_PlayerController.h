// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Types/DSTypes.h"
#include "DS_PlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimerStateChangedDelegate, float, Time, ECountdownTimerType, Type);
/** 1. 입력 통제
 *  2. 서버와 지연 시간을 계산하여 게임 카운트다운 시간 동기화
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADS_PlayerController();

	virtual void ReceivedPlayer() override;
	virtual void OnRep_PlayerState() override;
	virtual void PostSeamlessTravel() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void Client_TimerUpdated(float CountdownTimeLeft, ECountdownTimerType Type) const;

	UFUNCTION(Client, Reliable)
	void Client_TimerStopped(float CountdownTimeLeft, ECountdownTimerType Type) const;

	UFUNCTION(Client, Reliable)
	void Client_SetInputEnabled(bool bEnabled);

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangedDelegate OnTimerUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangedDelegate OnTimerStopped;

	void PlayerIsReadyForLobby(bool IsReady);
	
	UFUNCTION(Server, Reliable)
	void Server_PlayerIsReadyForLobby(bool IsReady);

	void PlayerIsReadyForMatch();

protected:
	UFUNCTION(Server, Reliable)
	void Server_Ping(float TimeOfRequest);

	UFUNCTION(Client, Reliable)
	void Client_Pong(float TimeOfRequest);

private:
	float SingleTripTime{};
};
























