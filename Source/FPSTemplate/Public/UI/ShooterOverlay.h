// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterOverlay.generated.h"

class UVerticalBox;
class UKillFeed;
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UKillFeed> KillFeedClass;

	UPROPERTY(EditDefaultsOnly)
	int32 KillFeedCount = 3;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_WaitingPlayersMessage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_KillFeedContainer;

	UFUNCTION()
	void UpdateWaitingPlayersMessage(bool IsStarted);

	void SetupWaitingPlayersMessage();

	UFUNCTION()
	void OnKillAnnounced(const FKillInfo& KillInfo);
	
};

