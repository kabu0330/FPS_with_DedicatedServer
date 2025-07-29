// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestHUD.generated.h"

class UOpenLevelWidget;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ATestHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOpenLevelWidget> OpenLevelWidgetClass;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UOpenLevelWidget> OpenLevelWidget;
};
