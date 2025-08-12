// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Lobby/LobbyPlayerInfo.h"
#include "DS_GameState.generated.h"

class ALobbyState;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyStateInitialized, ALobbyState*, LobbyState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerAdded, const FLobbyPlayerInfo&, PlayerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerRemoved, const FLobbyPlayerInfo&, PlayerInfo);

/** 1. LobbyLevel에서 현재 서버에 접속한 플레이어 리스트를 관리한다. 
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameState : public AGameState
{
	GENERATED_BODY()

public:
	ADS_GameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	//UPROPERTY(ReplicatedUsing = OnRep_LobbyState)
	//TObjectPtr<ALobbyState> LobbyState;

	//UPROPERTY(BlueprintAssignable)
	//FOnLobbyStateInitialized OnLobbyStateInitialized;
protected:
	virtual void BeginPlay() override;
	//void CreateLobbyState();

private:
	//UFUNCTION()
	//void OnRep_LobbyState();

public:
	FLobbyPlayerInfoArray& GetPlayerList();
	TArray<FLobbyPlayerInfo> GetPlayerListArray();

	UFUNCTION()
	void OnRep_PlayerList();
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerList) 
	FLobbyPlayerInfoArray PlayerList;

	UPROPERTY(BlueprintAssignable)
	FOnLobbyPlayerAdded OnPlayerAddedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnLobbyPlayerRemoved OnPlayerRemovedDelegate;
};
