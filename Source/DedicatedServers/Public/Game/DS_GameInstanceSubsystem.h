// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameLiftServerSDK.h"
#include "DS_GameInstanceSubsystem.generated.h"

struct FProcessParameters;
struct FServerParameters;
/** 1. InitGameLift() aws GameLift Servers 기능 지원
 *  2. 서버 트래블이 발생해도 GameLift Servers와 통신이 이어지도록 "ProcessParameters" 데이터 저장 
 */
UCLASS()
class DEDICATEDSERVERS_API UDS_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UDS_GameInstanceSubsystem();
	
	void InitGameLift(const FServerParameters& ServerParameters);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
private:
	TSharedPtr<FProcessParameters> ProcessParameters;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool bGameLiftInitialized;
	
	void BindCallback(FGameLiftServerSDKModule* GameLiftSdkModule);
	void ParesCommandLinePort();
};
