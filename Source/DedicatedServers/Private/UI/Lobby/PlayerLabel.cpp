// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/PlayerLabel.h"

#include "Components/TextBlock.h"

void UPlayerLabel::NativeConstruct()
{
	Super::NativeConstruct();
	TextBlock_Ready->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerLabel::SetUsername(const FString& Username) const
{
	TextBlock_Username->SetText(FText::FromString(Username));
}

FString UPlayerLabel::GetUsername() const
{
	return TextBlock_Username->GetText().ToString();
}

void UPlayerLabel::SetReady(bool bIsReady) const
{
	TextBlock_Ready->SetVisibility(bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}


