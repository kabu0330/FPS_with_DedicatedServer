// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShooterOverlay.h"

#include "Components/TextBlock.h"
#include "Game/DS_MatchGameState.h"


void UShooterOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_WaitingPlayersMessage->SetVisibility(ESlateVisibility::Visible);
	
	ADS_MatchGameState* MatchGameState = GetWorld()->GetGameState<ADS_MatchGameState>();
	if (!IsValid(MatchGameState)) return;
	MatchGameState->OnMatchCountdownStateChanged.AddDynamic(this, &UShooterOverlay::UpdateWaitingPlayersMessage);
	UE_LOG(LogTemp, Warning, TEXT("MatchGameState->OnMatchCountdownStateChanged.AddDynamic"));
}

void UShooterOverlay::UpdateWaitingPlayersMessage(bool IsStarted)
{
	const FString Bool = IsStarted? TEXT("true") : TEXT("false");
	UE_LOG(LogTemp, Warning, TEXT("UpdateWaitingPlayersMessage: %s"), *Bool);
	check(IsValid(TextBlock_WaitingPlayersMessage));
	TextBlock_WaitingPlayersMessage->SetVisibility(IsStarted ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}
