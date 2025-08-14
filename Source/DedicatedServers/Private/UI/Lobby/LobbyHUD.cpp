// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyHUD.h"

#include "Player/DS_PlayerController.h"
#include "UI/Lobby/LobbyOverlay.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = GetOwningPlayerController();
	LobbyOverlay = CreateWidget<ULobbyOverlay>(PlayerController, LobbyOverlayClass);
	if (LobbyOverlay)
	{
		LobbyOverlay->AddToViewport();
	}
	
	FInputModeGameAndUI InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}
