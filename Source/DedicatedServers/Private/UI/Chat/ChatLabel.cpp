// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/ChatLabel.h"

#include "Components/TextBlock.h"
#include "Game/ChatComponent.h"

void UChatLabel::AddMessage(const FChatMessage& Message)
{
	check(IsValid(TextBlock_Username));
	check(IsValid(TextBlock_Message));

	FString Username = Message.Username + TEXT(" :");
	TextBlock_Username->SetText(FText::FromString(Username));
	TextBlock_Message->SetText(Message.Message);
}
