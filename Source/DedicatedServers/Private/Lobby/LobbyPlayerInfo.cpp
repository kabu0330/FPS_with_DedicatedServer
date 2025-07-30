#include "Lobby/LobbyPlayerInfo.h"

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
