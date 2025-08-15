// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_MatchGameState.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/DS_PlayerController.h"

void ADS_MatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADS_MatchGameState, bIsMatchCountdownStarted);
}

void ADS_MatchGameState::OnRep_MatchCountdownStarted()
{
	const FString& Bool = bIsMatchCountdownStarted ? TEXT("true") : TEXT("false"); 
	UE_LOG(LogDedicatedServers, Warning, TEXT("MatchCountdownStarted: %s"), *Bool);
	OnMatchCountdownStateChanged.Broadcast(bIsMatchCountdownStarted);
}

