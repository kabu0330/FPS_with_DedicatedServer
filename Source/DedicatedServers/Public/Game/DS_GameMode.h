// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameLiftServerSDK.h"
#include "DS_GameMode.generated.h"

struct FProcessParameters;


/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
private:
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	void BindCallback(FGameLiftServerSDKModule* GameLiftSdkModule);
	void ParesCommandLinePort();
	
	TSharedPtr<FProcessParameters> ProcessParameters;
};
