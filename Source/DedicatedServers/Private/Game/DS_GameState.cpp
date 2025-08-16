// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameState.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/ChatComponent.h"
#include "Lobby/LobbyState.h"
#include "Net/UnrealNetwork.h"

ADS_GameState::ADS_GameState()
{
	bReplicates = true;

	ChatComponent = CreateDefaultSubobject<UChatComponent>(TEXT("ChatComponent"));
	ChatComponent->SetIsReplicated(true);
}

void ADS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADS_GameState, PlayerList);
}

UChatComponent* ADS_GameState::GetChatComponent() const
{
	return ChatComponent;
}

void ADS_GameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PlayerList.SetOwner(this);
	}
}

void ADS_GameState::OnRep_PlayerList()
{
	PlayerList.SetOwner(this);
	OnPlayerListUpdated.Broadcast();
}

FLobbyPlayerInfoArray& ADS_GameState::GetPlayerList()
{
	return PlayerList;
}

TArray<FLobbyPlayerInfo> ADS_GameState::GetPlayerListArray()
{
	return PlayerList.Items;
}

