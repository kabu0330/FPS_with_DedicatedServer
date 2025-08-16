// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatLabel.generated.h"

struct FChatMessage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UChatLabel : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddMessage(const FChatMessage& Message);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Message;
};
