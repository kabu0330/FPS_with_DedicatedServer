// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundGameState.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillAnnounced, const FKillInfo&, KillInfo);
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AMatchGameState : public ASoundGameState
{
	GENERATED_BODY()
public:
	AMatchGameState();

	AMatchPlayerState* GetLeader() const;

	void UpdateLeader();
	bool HasFirstBloodBeenHad() const { return bHasFirstBloodBeenHad; }
	bool IsTiedForTheLead(AMatchPlayerState* PlayerState);
	TArray<AMatchPlayerState*> GetLeaders() const;

	void AnnounceKill(const FKillInfo& KillInfo);

	UPROPERTY(BlueprintAssignable)
	FOnKillAnnounced OnKillAnnounced;
	
protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY()
	TArray<TObjectPtr<AMatchPlayerState>> Leaders;

	UPROPERTY()
	TArray<TObjectPtr<AMatchPlayerState>> SortedPlayerStates;

	bool bHasFirstBloodBeenHad;

	UFUNCTION(NetMulticast, reliable)
	void Multicast_AnnounceKill(const FKillInfo& KillInfo);
};
