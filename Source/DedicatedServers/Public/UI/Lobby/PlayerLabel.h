// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerLabel.generated.h"

class UTextBlock;
/** 1. Lobby의 스크롤 박스에 한줄씩 추가할 라벨
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UPlayerLabel : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetUsername(const FString& Username) const;
	FString GetUsername() const;
	void SetReady(bool bIsReady) const;

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Ready;
};
