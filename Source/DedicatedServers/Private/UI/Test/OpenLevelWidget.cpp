// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Test/OpenLevelWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UOpenLevelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	check(IsValid(Button_OpenLevel));

	Button_OpenLevel->OnClicked.AddDynamic(this, &UOpenLevelWidget::OpenLobbyLevel);
}

void UOpenLevelWidget::OpenLobbyLevel()
{
	const FString IpAndPort = TEXT("220.88.40.217:7777");
	const FString ServerIP = TEXT("127.0.0.1:7777");
	//GameplayStatics::OpenLevel(GetWorld(), *IpAndPort);
	UGameplayStatics::OpenLevel(GetWorld(), *ServerIP);
}
