// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameModeBase.h"
//#include "GameLiftServerSDK.h"
#include "ShooterGameMode.generated.h"

struct FProcessParameters;

DECLARE_LOG_CATEGORY_EXTERN(LogShooterGameMode, Log, All);

/**
 * 
 */
UCLASS(/*minimalapi*/)
class FPSTEMPLATE_API AShooterGameMode : public AShooterGameModeBase
{
	GENERATED_BODY()

public:
	AShooterGameMode();

protected:
	virtual void BeginPlay() override;

private:
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	
	TSharedPtr<FProcessParameters> ProcessParameters;
};

