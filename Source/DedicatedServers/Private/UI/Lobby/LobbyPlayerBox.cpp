// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyPlayerBox.h"

#include "Components/ScrollBox.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameState.h"
#include "Lobby/LobbyState.h"
#include "UI/Lobby/PlayerLabel.h"
#include "Game/DS_LobbyGameMode.h"

void ULobbyPlayerBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ADS_GameState* GameState = GetWorld()->GetGameState<ADS_GameState>();
	if (!IsValid(GameState)) return;
	GameState->OnPlayerListUpdated.AddDynamic(this, &ULobbyPlayerBox::UpdatePlayerList);
	
	UpdatePlayerList();
}

UPlayerLabel* ULobbyPlayerBox::FindPlayerLabel(const FString& Username)
{
	for (UWidget* Child : ScrollBox_PlayerInfo->GetAllChildren())
	{
		UPlayerLabel* PlayerLabel = Cast<UPlayerLabel>(Child);
		if (IsValid(PlayerLabel) && PlayerLabel->GetUsername() == Username)
		{
			return PlayerLabel;
		}
	}
	return nullptr;
}

void ULobbyPlayerBox::UpdatePlayerList()
{
	ADS_GameState* GameState = GetWorld()->GetGameState<ADS_GameState>();
	if (!IsValid(GameState))
	{
		ScrollBox_PlayerInfo->ClearChildren();
		return;
	}

	TArray<FLobbyPlayerInfo> PlayerInfos = GameState->GetPlayerListArray();
	TArray<UWidget*> PlayerLabelsToRemove;
	
	for (UWidget* Child : ScrollBox_PlayerInfo->GetAllChildren())
	{
		UPlayerLabel* PlayerLabel = Cast<UPlayerLabel>(Child);
		if (IsValid(PlayerLabel))
		{
			const bool bPlayerDataExists = PlayerInfos.ContainsByPredicate(
				[&](const FLobbyPlayerInfo& Info)
			{
				return Info.Username == PlayerLabel->GetUsername();
			});

			if (!bPlayerDataExists)
			{
				PlayerLabelsToRemove.Add(PlayerLabel);
			}
		}
	}
	
	for (UWidget* LabelToRemove : PlayerLabelsToRemove)
	{
		ScrollBox_PlayerInfo->RemoveChild(LabelToRemove);
	}
	
	for (const FLobbyPlayerInfo& PlayerInfo : PlayerInfos)
	{
		UPlayerLabel* PlayerLabel = FindPlayerLabel(PlayerInfo.Username);
		if (IsValid(PlayerLabel)) // 준비 상태만 업데이트
		{
			PlayerLabel->SetReady(PlayerInfo.bIsReady);
		}
		else // 라벨 생성
		{
			UPlayerLabel* NewPlayerLabel = CreateWidget<UPlayerLabel>(this, PlayerLabelClass);
			if (IsValid(NewPlayerLabel))
			{
				NewPlayerLabel->SetUsername(PlayerInfo.Username);
				NewPlayerLabel->SetReady(PlayerInfo.bIsReady);
				ScrollBox_PlayerInfo->AddChild(NewPlayerLabel);
			}
		}
	}
}











