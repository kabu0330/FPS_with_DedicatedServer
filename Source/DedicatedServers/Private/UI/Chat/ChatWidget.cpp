// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/ChatWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Game/ChatComponent.h"
#include "Game/DS_GameState.h"
#include "Player/DS_PlayerController.h"
#include "UI/Chat/ChatLabel.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ADS_GameState* GS = GetWorld()->GetGameState<ADS_GameState>(); IsValid(GS))
	{
		if (UChatComponent* ChatComponent = GS->GetChatComponent())
		{
			UE_LOG(LogTemp, Warning, TEXT("ChatComponent is valid"));
			ChatComponent->OnChatMessageReceived.AddDynamic(this, &UChatWidget::ChatMessageReceived);
		}
	}
	
	if (ADS_PlayerController* PC = GetOwningPlayer<ADS_PlayerController>(); IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is valid"));
		PC->OnChatStatus.AddDynamic(this, &UChatWidget::ReadyToChatMode);
	}
}

void UChatWidget::ChatMessageReceived(const FChatMessage& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("ChatMessageReceived %s : %s"), *Message.Username, *Message.Message.ToString());
	UChatLabel* NewChat = CreateWidget<UChatLabel>(this, ChatLabelClass);
	if (!IsValid(NewChat)) return;
	
	NewChat->AddMessage(Message);
	ScrollBox_ChatMessages->AddChild(NewChat);
}

void UChatWidget::ReadyToChatMode(bool IsChatMode)
{
	if (!IsValid(TextBox_ChatInputMessage)) return;
	APlayerController* PC = GetOwningPlayer();
	if (IsChatMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChatMode"));
		FInputModeUIOnly InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetWidgetToFocus(TextBox_ChatInputMessage->TakeWidget());
		PC->SetInputMode(InputModeData);
		PC->SetShowMouseCursor(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not ChatMode"));
		FInputModeGameOnly InputModeData;
		PC->SetInputMode(InputModeData);
		PC->SetShowMouseCursor(false);
	}
}

void UChatWidget::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("OnChatTextCommitted %s"), *Text.ToString());
	if (CommitInfo == ETextCommit::Type::OnEnter && !Text.IsEmpty())
	{
		if (ADS_PlayerController* PC = GetOwningPlayer<ADS_PlayerController>(); IsValid(PC))
		{
			PC->Server_SendChatMessage(Text);
			TextBox_ChatInputMessage->SetText(FText::GetEmpty());	
		}
	}
}
