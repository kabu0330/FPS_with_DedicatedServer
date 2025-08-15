// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyOverlay.h"

#include "Components/Button.h"
#include "Game/DS_GameState.h"
#include "Player/DS_DefaultPlayerState.h"
#include "Player/DS_MatchPlayerState.h"
#include "Player/DS_PlayerController.h"

void ULobbyOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Ready->OnClicked.AddDynamic(this, &ULobbyOverlay::ReadyButtonClicked);
}

void ULobbyOverlay::ReadyButtonClicked()
{
	if (ADS_PlayerController* PC = GetOwningPlayer<ADS_PlayerController>(); IsValid(PC))
	{
		ADS_GameState* GameState = PC->GetWorld()->GetGameState<ADS_GameState>();
		ADS_MatchPlayerState* PlayerState = PC->GetPlayerState<ADS_MatchPlayerState>();
		if (!IsValid(GameState) && !IsValid(PlayerState)) return;
		
		bool bCurrentIsReady = false;
		const FString Username = PlayerState->GetUsername();

		for (const FLobbyPlayerInfo& PlayerInfo : GameState->GetPlayerListArray())
		{
			if (PlayerInfo.Username == Username)
			{
				bCurrentIsReady = !PlayerInfo.bIsReady;
				break;
			}
		}
		
		PlayerState->PlayerIsReadyForLobby(bCurrentIsReady);
	}
}
