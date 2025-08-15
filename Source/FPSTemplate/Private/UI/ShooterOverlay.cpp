// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShooterOverlay.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Game/DS_MatchGameState.h"
#include "Game/MatchGameState.h"
#include "UI/Elims/KillFeed.h"


void UShooterOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetupWaitingPlayersMessage();
	
	AMatchGameState* GS = GetWorld()->GetGameState<AMatchGameState>();
	check(IsValid(GS));
	GS->OnKillAnnounced.AddDynamic(this, &UShooterOverlay::OnKillAnnounced);
	VerticalBox_KillFeedContainer->ClearChildren();
}

void UShooterOverlay::SetupWaitingPlayersMessage()
{
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

void UShooterOverlay::OnKillAnnounced(const FKillInfo& KillInfo)
{
	check(IsValid(KillFeedClass));

	if (VerticalBox_KillFeedContainer->GetChildrenCount() >= KillFeedCount)
	{
		VerticalBox_KillFeedContainer->GetChildAt(0)->RemoveFromParent();
	}

	UKillFeed* NewKillFeed = CreateWidget<UKillFeed>(this, KillFeedClass);
	if (NewKillFeed)
	{
		NewKillFeed->SetKillInfo(KillInfo);
		NewKillFeed->StartDisappearTimer(); // n초 뒤 소멸
		VerticalBox_KillFeedContainer->AddChild(NewKillFeed);
	}
}


