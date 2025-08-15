// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DS_DefaultPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDataChanged, const FString&, PlayerData);
/** 1. 플레이어의 기본 정보를 저장
 *  
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_DefaultPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADS_DefaultPlayerState();
	
	FString GetUsername() const;
	FString GetPlayerSessionId() const;

	void SetUsername(const FString& Username);
	void SetPlayerSessionId(const FString& SessionId);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDataChanged OnPlayerUsernameChanged;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OverrideWith(APlayerState* PlayerState) override;
	
	UPROPERTY(ReplicatedUsing = OnRep_Username)
	FString DefaultUsername = "";

	UPROPERTY(ReplicatedUsing = OnRep_PlayerSessionId)
	FString DefaultPlayerSessionId = "";

private:
	UFUNCTION()
	void OnRep_Username();

	UFUNCTION()
	void OnRep_PlayerSessionId();
	
};
