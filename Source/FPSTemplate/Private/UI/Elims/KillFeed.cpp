// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elims/KillFeed.h"

#include "Components/TextBlock.h"
#include "Game/MatchGameState.h"

void UKillFeed::SetKillInfo(const FKillInfo& KillInfo)
{
	const FString& KillerName = KillInfo.KillerName;
	const FString& VictimName = KillInfo.VictimName;

	check(IsValid(TextBlock_KillerName));
	check(IsValid(TextBlock_VictimName));

	TextBlock_KillerName->SetText(FText::FromString(KillerName));
	TextBlock_VictimName->SetText(FText::FromString(VictimName));
}

void UKillFeed::StartDisappearTimer()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&UKillFeed::PlayFadeOut,
		Duration,
		false
		);
}

void UKillFeed::PlayFadeOut()
{
	if (FadeOutAnimation)
	{
		FWidgetAnimationDynamicEvent FinishDelegate;
		FinishDelegate.BindUFunction(this, FName("OnFadeOutFinished"));
		BindToAnimationFinished(FadeOutAnimation, FinishDelegate);

		PlayAnimation(FadeOutAnimation);
	}
	else
	{
		RemoveFromParent();
	}
}

void UKillFeed::OnFadeOutFinished()
{
	RemoveFromParent();
}
