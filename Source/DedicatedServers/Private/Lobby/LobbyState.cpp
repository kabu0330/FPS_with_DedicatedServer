// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyState.h"
#include "Net/UnrealNetwork.h"

ALobbyState::ALobbyState()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true; // 거리에 상관없이 모든 클라에게 복제되어야 함을 명시
}

void ALobbyState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyState, PlayerInfoArray);
}

void ALobbyState::AddPlayerInfo(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerInfoArray.AddPlayer(PlayerInfo);
}

void ALobbyState::RemovePlayerInfo(const FString& PlayerInfo)
{
	PlayerInfoArray.RemovePlayer(PlayerInfo);
}

TArray<FLobbyPlayerInfo> ALobbyState::GetPlayers() const
{
	return PlayerInfoArray.Items;
}

void ALobbyState::OnRep_LobbyPlayerInfo()
{
	FLobbyPlayerInfoDelta Delta = ComputePlayerInfoDelta(LastPlayerInfoArray.Items, PlayerInfoArray.Items);
	for (const auto& PlayerInfo : Delta.AddPlayers)
	{
		OnPlayerInfoAdded.Broadcast(PlayerInfo);
	}
	for (const auto& PlayerInfo : Delta.RemovedPlayers)
	{
		OnPlayerInfoRemoved.Broadcast(PlayerInfo);
	}
	
	LastPlayerInfoArray = PlayerInfoArray;
}

FLobbyPlayerInfoDelta ALobbyState::ComputePlayerInfoDelta(const TArray<FLobbyPlayerInfo>& OldArray,
	const TArray<FLobbyPlayerInfo>& NewArray)
{
	FLobbyPlayerInfoDelta Delta;

	TMap<FString, const FLobbyPlayerInfo*> OldMap;
	TMap<FString, const FLobbyPlayerInfo*> NewMap;

	for (const auto& PlayerInfo : OldArray)
	{
		OldMap.Add(PlayerInfo.Username, &PlayerInfo);	
	}
	for (const auto& PlayerInfo : NewArray)
	{
		NewMap.Add(PlayerInfo.Username, &PlayerInfo);
	}
	
	for (const auto& OldPlayerInfo : OldArray)
	{
		if (!NewMap.Contains(OldPlayerInfo.Username)) // 신규 배열에서 사라진 Username이 있다면 삭제된 것
		{
			Delta.RemovedPlayers.Add(OldPlayerInfo);
		}
	}
	for (const auto& NewPlayerInfo : NewArray) // 기존 배열에 없던 Username이 있으면 신규 사용자가 있는 것
	{
		if (!OldMap.Contains(NewPlayerInfo.Username))
		{
			Delta.AddPlayers.Add(NewPlayerInfo);
		}
	}
	
	return Delta;
}



