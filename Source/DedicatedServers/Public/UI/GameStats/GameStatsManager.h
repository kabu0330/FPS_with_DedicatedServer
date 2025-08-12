// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "GameStatsManager.generated.h"

struct FDSLeaderboardItem;
struct FDSRetrieveMatchStatsResponse;
struct FDSRecordMatchStatsInput;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrieveMatchStatsResponse, const FDSRetrieveMatchStatsResponse&, MatchStatsResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrieveLeaderboard, TArray<FDSLeaderboardItem>&, Leaderboard);
/** 1. 플레이 기록(Record) 및 조회
 *  2. 랭킹 기록 및 조회
 */
UCLASS()
class DEDICATEDSERVERS_API UGameStatsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	void RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput);
	void RetrieveMatchStats();
	void UpdateLeaderboard(const TArray<FString>& PlayerNames);
	void RetrieveLeaderboard();
	

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessgae RetrieveMatchStatsStatusMessage;

	UPROPERTY(BlueprintAssignable)
	FOnRetrieveMatchStatsResponse OnRetrieveMatchStatsResponse;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnUpdatedGameStatsSucceeded;
	
	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnUpdatedLeaderboardSucceeded;

	UPROPERTY(BlueprintAssignable)
	FOnRetrieveLeaderboard OnRetrieveLeaderboard;

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessgae RetrieveLeaderboardMessage;
	
private:
	void RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void UpdateLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RetrieveLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
