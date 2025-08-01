// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dropdown.h"
#include "AccountDropdown.generated.h"

class UDS_LocalPlayerSubsystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAccountDropdown : public UDropdown
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UDS_LocalPlayerSubsystem* GetLocalPlayerSubsystem() const;
	
};
