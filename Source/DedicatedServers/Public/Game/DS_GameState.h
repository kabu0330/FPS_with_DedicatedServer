// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Lobby/LobbyPlayerInfo.h"
#include "DS_GameState.generated.h"

class UChatComponent;
class ALobbyState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerListUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatComponentCreated, UChatComponent*, ChatComponent);

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

	FLobbyPlayerInfoArray& GetPlayerList();
	TArray<FLobbyPlayerInfo> GetPlayerListArray();
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerListUpdated OnPlayerListUpdated;

	UChatComponent* GetChatComponent() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerList) 
	FLobbyPlayerInfoArray PlayerList;

	UPROPERTY()
	TObjectPtr<UChatComponent> ChatComponent;
	
	UFUNCTION()
	void OnRep_PlayerList();

};
