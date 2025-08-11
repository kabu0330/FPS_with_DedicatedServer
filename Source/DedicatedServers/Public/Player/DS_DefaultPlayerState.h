// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DS_DefaultPlayerState.generated.h"

/**
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

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OverrideWith(APlayerState* PlayerState) override;
	
	UPROPERTY(ReplicatedUsing = OnRep_DisplayUsername)
	FString DefaultUsername = "";

	UPROPERTY(ReplicatedUsing = OnRep_DisplayPlayerSessionId)
	FString DefaultPlayerSessionId = "";

private:
	UFUNCTION()
	void OnRep_DisplayUsername();

	UFUNCTION()
	void OnRep_DisplayPlayerSessionId();
	
};
