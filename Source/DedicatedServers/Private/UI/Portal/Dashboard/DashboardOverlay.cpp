// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/DashboardOverlay.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/GameStats/GameStatsManager.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/Portal/Dashboard/LeaderboardPage.h"

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(IsValid(GameStatsManagerClass));
	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	
	GameStatsManager->OnRetrieveMatchStatsResponse.AddDynamic(CareerPage, &UCareerPage::OnRetrieveMatchStats);
	GameStatsManager->RetrieveMatchStatsStatusMessage.AddDynamic(CareerPage, &UCareerPage::SetStatusMessage);
	
	GameStatsManager->OnRetrieveLeaderboard.AddDynamic(LeaderboardPage, &ULeaderboardPage::PopulateLeaderboard);
	GameStatsManager->RetrieveLeaderboardMessage.AddDynamic(LeaderboardPage, &ULeaderboardPage::SetStatusMessage);
	
	Button_Game->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
	Button_Career->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
	Button_Leaderboard->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);

	ShowGamePage();
}

void UDashboardOverlay::ShowGamePage()
{
	DisableButtons(Button_Game);
	WidgetSwitcher->SetActiveWidget(GamePage);
}

void UDashboardOverlay::ShowCareerPage()
{
	DisableButtons(Button_Career);
	WidgetSwitcher->SetActiveWidget(CareerPage);
	GameStatsManager->RetrieveMatchStats();
}

void UDashboardOverlay::ShowLeaderboardPage()
{
	DisableButtons(Button_Leaderboard);
	WidgetSwitcher->SetActiveWidget(LeaderboardPage);
	GameStatsManager->RetrieveLeaderboard();
}

void UDashboardOverlay::DisableButtons(UButton* Button) const
{
	Button_Game->SetIsEnabled(true);
	Button_Career->SetIsEnabled(true);
	Button_Leaderboard->SetIsEnabled(true);
	
	Button->SetIsEnabled(false);
}



