// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

class UChatLabel;
struct FChatMessage;
class UScrollBox;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitInfo);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatLabel> ChatLabelClass;
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_ChatInputMessage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ChatMessages;

	UFUNCTION()
	void ChatMessageReceived(const FChatMessage& Message);

	UFUNCTION()
	void ReadyToChatMode(bool IsChatMode);
};
