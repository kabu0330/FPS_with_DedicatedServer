// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "DS_GameMode.generated.h"

struct FProcessParameters;


/** GameLift SDK 초기 설정만 책임진다.
 *  더 이상 사용하지 않음
 *  LobbyGameMode와 GameInstanceSubsystem으로 기능 이관
 */ 
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
private:
	TSharedPtr<FProcessParameters> ProcessParameters;
	
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	void BindCallback(FGameLiftServerSDKModule* GameLiftSdkModule);
	void ParesCommandLinePort();
};
