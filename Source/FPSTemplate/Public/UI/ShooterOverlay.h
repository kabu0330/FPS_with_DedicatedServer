// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterOverlay.generated.h"

class UTextBlock;
class APlayerState;
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UShooterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_WaitingPlayersMessage;

	UFUNCTION()
	void UpdateWaitingPlayersMessage(bool IsStarted);
	
};

