// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillFeed.generated.h"

struct FKillInfo;
class UTextBlock;
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UKillFeed : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetKillInfo(const FKillInfo& KillInfo);

	void StartDisappearTimer();

	UPROPERTY(EditDefaultsOnly)
	float Duration = 5.0f;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_KillerName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_VictimName;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnimation;

	UFUNCTION()
	void PlayFadeOut();

	UFUNCTION()
	void OnFadeOutFinished();
};
