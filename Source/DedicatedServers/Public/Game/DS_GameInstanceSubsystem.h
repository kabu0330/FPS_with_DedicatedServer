// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameLiftServerSDK.h"
#include "DS_GameInstanceSubsystem.generated.h"

struct FProcessParameters;
struct FServerParameters;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDS_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UDS_GameInstanceSubsystem();
	
	void InitGameLift(const FServerParameters& ServerParameters);

private:
	UPROPERTY()
	TSharedPtr<FProcessParameters> ProcessParameters;

	UPROPERTY(BlueprintReadOnly, Category = "GameLift")
	bool bGameLiftInitialized;

	void BindCallback(FGameLiftServerSDKModule* GameLiftSdkModule);
	void ParesCommandLinePort();
};
