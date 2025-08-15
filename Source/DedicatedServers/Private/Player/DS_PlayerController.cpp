// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_PlayerController.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameState.h"
#include "Game/DS_LobbyGameMode.h"
#include "Game/DS_MatchGameMode.h"
#include "Player/DS_DefaultPlayerState.h"
#include "Player/DS_MatchPlayerState.h"

ADS_PlayerController::ADS_PlayerController()
{
}

void ADS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_Standalone)
	{
		if (GIsEditor) // Debug
		{
			EnableInput(this);
		}
		else
		{
			DisableInput(this);
		}
	}
}

void ADS_PlayerController::UpdateAllNameplatesOnClient()
{
	
}

void ADS_PlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsLocalController())
	{
		DisableInput(this);
		
		OnPlayerStateCreated.Broadcast(PlayerState);
	}
}

void ADS_PlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	ADS_MatchPlayerState* PS = Cast<ADS_MatchPlayerState>(PlayerState);
	check(IsValid(PS));
	PS->PlayerIsReadyForMatch();
	
	if (IsLocalPlayerController())
	{
		Server_Ping(GetWorld()->GetTimeSeconds());
		DisableInput(this);
	}
}

void ADS_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ADS_PlayerController::Client_SetInputEnabled_Implementation(bool bEnabled)
{
	if (bEnabled)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
}

void ADS_PlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (GetNetMode() == NM_Standalone) return;
	if (IsLocalPlayerController())
	{
		Server_Ping(GetWorld()->GetTimeSeconds());
	}
}

void ADS_PlayerController::Server_Ping_Implementation(float TimeOfRequest)
{
	// TimeOfRequest = Getworld()->GetDeltaSeconds()
	Client_Pong(TimeOfRequest);	
}

void ADS_PlayerController::Client_Pong_Implementation(float TimeOfRequest)
{
	// 서버에서 타이머를 호출하면 클라와 타이머를 동기화 방법으로
	// 클라가 핑을 보내면, 즉시 서버가 퐁을 보내서 "두 지연시간의 합 / 2 = 평균 지연시간"으로 판단
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfRequest;
	SingleTripTime = RoundTripTime * 0.5f; // 근사치
}

void ADS_PlayerController::Client_TimerUpdated_Implementation(float CountdownTimeLeft, ECountdownTimerType Type) const
{
	// 서버에서 클라로 넘어오는 지연시간 근사치만큼 남은 시간에 추가하여 감소
	// 서버 : 야 4초 남았어!
	// 클라 : 4초라고 전달해준게 0.2초 전이니까 지금 3.8초 남았다고 전파할게.
	OnTimerUpdated.Broadcast(CountdownTimeLeft - SingleTripTime, Type);
}

void ADS_PlayerController::Client_TimerStopped_Implementation(float CountdownTimeLeft, ECountdownTimerType Type) const
{
	// Type == Stopped, 즉시 카운트다운 종료 전파
	OnTimerStopped.Broadcast(CountdownTimeLeft - SingleTripTime, Type);
}


