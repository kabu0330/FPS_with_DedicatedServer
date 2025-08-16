// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Types/DSTypes.h"
#include "DS_PlayerController.generated.h"


struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimerStateChangedDelegate, float, Time, ECountdownTimerType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchCountdownTimerStart, bool, IsStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateCreated, APlayerState*, PS);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatStatus, bool, IsChatting);
/** 1. 입력 통제
 *  2. 서버와 지연 시간을 계산하여 게임 카운트다운 시간 동기화
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADS_PlayerController();

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

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateCreated OnPlayerStateCreated;

	UPROPERTY(BlueprintAssignable)
	FOnChatStatus OnChatStatus;

	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(const FText& Message);

protected:
	virtual void ReceivedPlayer() override;
	virtual void OnRep_PlayerState() override;
	virtual void PostSeamlessTravel() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UFUNCTION(Server, Reliable)
	void Server_Ping(float TimeOfRequest);

	UFUNCTION(Client, Reliable)
	void Client_Pong(float TimeOfRequest);

private:
	float SingleTripTime{};

	bool bIsChatting = false;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DSMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ChatAction;

	void Input_StartChat(const FInputActionValue& InputActionValue);
	
};
























