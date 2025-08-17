// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DS_MatchGameState.h"
#include "SoundGameState.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API ASoundGameState : public ADS_MatchGameState
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void Server_PlaySoundOnFirstBlood();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySoundOnFirstBlood();

	UFUNCTION(Server, Reliable)
	void Server_PlaySoundOnMultiKill(int32 MultiKillCount);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySoundOnMultiKill(int32 MultiKillCount);

	UFUNCTION()
	void PlaySoundOnKill();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> FirstBloodSound;

	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> KillSound;

	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> KillSound2;

	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> KillSound3;

	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> KillSound4;
	

	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> DoubleKillSound;

	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> TripleKillSound;

	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> QuadKillSound;

	UPROPERTY(EditDefaultsOnly, Category = "GameSound")
	TObjectPtr<USoundBase> PentaKillSound;


private:

};
