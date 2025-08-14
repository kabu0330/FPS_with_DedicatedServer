#include "Lobby/LobbyPlayerInfo.h"

#include "Game/DS_GameState.h"

void FLobbyPlayerInfo::TriggerUpdate(const FLobbyPlayerInfoArray& InArraySerializer)
{
	if (!InArraySerializer.GameState) return;

	if (ADS_GameState* GameState = Cast<ADS_GameState>(InArraySerializer.GetOwner()))
	{
		GameState->OnPlayerListUpdated.Broadcast();
	}
}

void FLobbyPlayerInfo::PostReplicatedAdd(const FLobbyPlayerInfoArray& InArraySerializer)
{
	TriggerUpdate(InArraySerializer);
}

void FLobbyPlayerInfo::PreReplicatedRemove(const FLobbyPlayerInfoArray& InArraySerializer)
{
	TriggerUpdate(InArraySerializer);
}

void FLobbyPlayerInfo::PostReplicatedChange(const FLobbyPlayerInfoArray& InArraySerializer)
{
	TriggerUpdate(InArraySerializer);
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

void FLobbyPlayerInfoArray::SetPlayerReady(const FString& Username, bool IsReady)
{
	for (FLobbyPlayerInfo& PlayerInfo : Items)
	{
		if (PlayerInfo.Username == Username)
		{
			PlayerInfo.bIsReady = IsReady;
			MarkItemDirty(PlayerInfo);
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
