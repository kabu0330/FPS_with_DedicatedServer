// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DS_MatchGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchCountdownStateChanged, bool, bIsStart);

/** 
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnMatchCountdownStateChanged OnMatchCountdownStateChanged;

	UPROPERTY(ReplicatedUsing = OnRep_MatchCountdownStarted)
	bool bIsMatchCountdownStarted = false;

	UFUNCTION()
	void OnRep_MatchCountdownStarted();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
