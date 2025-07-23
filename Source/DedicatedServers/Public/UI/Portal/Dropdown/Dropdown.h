﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dropdown.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UWidgetSwitcher;
/** 계정 정보 제공 UI
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDropdown : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UUserWidget> ExpandedWidget;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UUserWidget> CollapsedWidget;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> Button_Expander;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown" )
	FSlateColor HoveredTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown" )
	FSlateColor UnhoveredTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown" )
	FSlateBrush Triangle_Up;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown" )
	FSlateBrush Triangle_Down;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UImage> Image_Triangle;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

private:
	bool bIsExpanded;

	void Expand();
	void Collapse();

	UFUNCTION()
	void Hover();

	UFUNCTION()
	void Unhover();

	void SetStyleTransparent();

	UFUNCTION()
	void ToggleDropdown();
};
