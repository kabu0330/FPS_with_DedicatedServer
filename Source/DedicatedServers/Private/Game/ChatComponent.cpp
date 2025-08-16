// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ChatComponent.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Player/DS_DefaultPlayerState.h"


UChatComponent::UChatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UChatComponent::SendChatMessage(const APlayerController* Sender, const FText& Message)
{
	if (GetOwner()->HasAuthority())
	{
		BroadcastChatMessage(Sender, Message);
	}
}

void UChatComponent::BroadcastChatMessage(const APlayerController* Sender, const FText& Message)
{
	if (!Sender || Message.IsEmpty()) return;
	
	FChatMessage ChatMessage;
	if (ADS_DefaultPlayerState* PS = Sender->GetPlayerState<ADS_DefaultPlayerState>(); IsValid(PS))
	{
		ChatMessage.Username = PS->GetUsername();
	}
	ChatMessage.Message = Message;

	UE_LOG(LogDedicatedServers, Log, TEXT("Chat %s : %s"), *ChatMessage.Username, *ChatMessage.Message.ToString());
	Multicast_RelayChatMessage(ChatMessage);
}

void UChatComponent::Multicast_RelayChatMessage_Implementation(const FChatMessage& ChatMessage)
{
	// ChatWidget에게 델리게이트 전파
	UE_LOG(LogDedicatedServers, Log, TEXT("Multicast_RelayChatMessage %s : %s"), *ChatMessage.Username, *ChatMessage.Message.ToString());
	OnChatMessageReceived.Broadcast(ChatMessage);
}

void UChatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UChatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

