// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DS_MatchGameState.h"
#include "GameFramework/GameState.h"
#include "MatchGameState.generated.h"

class AMatchPlayerState;

USTRUCT(BlueprintType)
struct FKillInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString KillerName;

	UPROPERTY()
	FString VictimName;
};
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AMatchGameState : public ADS_MatchGameState
{
	GENERATED_BODY()
public:
	AMatchGameState();

	AMatchPlayerState* GetLeader() const;

	void UpdateLeader();
	bool HasFirstBloodBeenHad() const { return bHasFirstBloodBeenHad; }
	bool IsTiedForTheLead(AMatchPlayerState* PlayerState);
	TArray<AMatchPlayerState*> GetLeaders() const;
	
protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY()
	TArray<TObjectPtr<AMatchPlayerState>> Leaders;

	UPROPERTY()
	TArray<TObjectPtr<AMatchPlayerState>> SortedPlayerStates;

	bool bHasFirstBloodBeenHad;
};
