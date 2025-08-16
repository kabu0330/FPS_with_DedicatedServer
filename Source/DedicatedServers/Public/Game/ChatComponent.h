// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatComponent.generated.h"

USTRUCT()
struct FChatMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString Username;

	UPROPERTY()
	FText Message;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatMessageReceived, const FChatMessage&, ChatMessage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEDICATEDSERVERS_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChatComponent();

	void SendChatMessage(const APlayerController* Sender, const FText& Message);

	UPROPERTY(BlueprintAssignable)
	FOnChatMessageReceived OnChatMessageReceived;

protected:
	virtual void BeginPlay() override;

	void BroadcastChatMessage(const APlayerController* Sender, const FText& Message);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RelayChatMessage(const FChatMessage& ChatMessage);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
