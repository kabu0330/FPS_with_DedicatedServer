// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dropdown/AccountDropdown.h"

#include "Components/TextBlock.h"
#include "Player/DS_LocalPlayerSubsystem.h"

void UAccountDropdown::NativeConstruct()
{
	Super::NativeConstruct();

	UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		TextBlock_ButtonText->SetText(FText::FromString(LocalPlayerSubsystem->GetUsername()));
	}
}

UDS_LocalPlayerSubsystem* UAccountDropdown::GetLocalPlayerSubsystem() const
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(PlayerController->GetLocalPlayer()))
	{
		UDS_LocalPlayerSubsystem* LocalPlayerSubsystem= PlayerController->GetLocalPlayer()->GetSubsystem<UDS_LocalPlayerSubsystem>();
		if (IsValid(LocalPlayerSubsystem))
		{
			return LocalPlayerSubsystem;
		}
	}
	return nullptr;
}
