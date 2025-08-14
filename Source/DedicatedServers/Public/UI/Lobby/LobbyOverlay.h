// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyOverlay.generated.h"

class UButton;
class ULobbyPlayerBox;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULobbyOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULobbyPlayerBox> LobbyPlayerBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Ready;

private:
	UFUNCTION()
	void ReadyButtonClicked();
};
