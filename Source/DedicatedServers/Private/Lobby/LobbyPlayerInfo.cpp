#include "Lobby/LobbyPlayerInfo.h"

#include "Game/DS_GameState.h"

void FLobbyPlayerInfo::PostReplicatedAdd(const FLobbyPlayerInfoArray& InArraySerializer)
{
	if (ADS_GameState* GameState = Cast<ADS_GameState>(InArraySerializer.GetOwner()))
	{
		GameState->OnPlayerAddedDelegate.Broadcast(*this);
	}
}

void FLobbyPlayerInfo::PreReplicatedRemove(const FLobbyPlayerInfoArray& InArraySerializer)
{
	if (ADS_GameState* GameState = Cast<ADS_GameState>(InArraySerializer.GetOwner()))
	{
		GameState->OnPlayerRemovedDelegate.Broadcast(*this);
	}
}

void FLobbyPlayerInfoArray::AddPlayer(const FLobbyPlayerInfo& NewPlayerInfo)
{
	int32 Index = Items.Add(NewPlayerInfo);
	MarkItemDirty(Items[Index]);
	Items[Index].PostReplicatedAdd(*this);
}

void FLobbyPlayerInfoArray::RemovePlayer(const FString& Username)
{
	for (int32 PlayerIndex = 0; PlayerIndex < Items.Num(); ++PlayerIndex)
	{
		FLobbyPlayerInfo& PlayerInfo = Items[PlayerIndex];
		if (PlayerInfo.Username == Username)
		{
			PlayerInfo.PreReplicatedRemove(*this);
			Items.RemoveAtSwap(PlayerIndex);
			MarkArrayDirty();
			break;
		}
	}
}

AGameState* FLobbyPlayerInfoArray::GetOwner() const
{
	return GameState;
}

void FLobbyPlayerInfoArray::SetOwner(AGameState* InGameState)
{
	GameState = InGameState;
}
