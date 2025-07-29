// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Test/TestHUD.h"

#include "UI/Test/OpenLevelWidget.h"

void ATestHUD::BeginPlay()
{
	Super::BeginPlay();

	check(IsValid(OpenLevelWidgetClass));
	OpenLevelWidget = CreateWidget<UOpenLevelWidget>(GetOwningPlayerController(), OpenLevelWidgetClass);
	if (OpenLevelWidget)
	{
		OpenLevelWidget->AddToViewport();
	}
	
}
