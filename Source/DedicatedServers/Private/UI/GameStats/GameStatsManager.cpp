// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStats/GameStatsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Player/DS_LocalPlayerSubsystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UGameStatsManager::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(
		FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonString
		);
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats);
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RecordMatchStats_Response);
	Request->SetURL(APIUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
}

void UGameStatsManager::RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                                  bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("RecordMatchStats_Response Failed..."));
	}
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		ContainsErrors(JsonObject, true);
	}
}

void UGameStatsManager::RetrieveMatchStats()
{
	RetrieveMatchStatsStatusMessage.Broadcast(TEXT("플레이어 데이터 검색 중..."), false);
	
	UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (!IsValid(LocalPlayerSubsystem)) return;
	check(APIData);

	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RetrieveMatchStats);
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveMatchStats_Response);
	Request->SetURL(APIUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	
	TMap<FString, FString> Params = {
		{TEXT("accessToken"), LocalPlayerSubsystem->GetAuthenticationResult().AccessToken}
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UGameStatsManager::RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                                    bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnRetrieveMatchStatsResponse.Broadcast(FDSRetrieveMatchStatsResponse());
		RetrieveMatchStatsStatusMessage.Broadcast(TEXT("데이터를 가져오는데 실패했습니다. (fail)"), false);
		UE_LOG(LogDedicatedServers, Error, TEXT("RetrieveMatchStats_Response Failed..."));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			OnRetrieveMatchStatsResponse.Broadcast(FDSRetrieveMatchStatsResponse());
			RetrieveMatchStatsStatusMessage.Broadcast(TEXT("데이터를 가져오는데 실패했습니다. (error)"), false);
			return;
		}

		FDSRetrieveMatchStatsResponse RetrieveMatchStatsResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RetrieveMatchStatsResponse);
		RetrieveMatchStatsResponse.Dump();
		
		OnRetrieveMatchStatsResponse.Broadcast(RetrieveMatchStatsResponse);
		RetrieveMatchStatsStatusMessage.Broadcast(TEXT(""), false);
	}
}

void UGameStatsManager::UpdateLeaderboard(const TArray<FString>& WinnerUsername)
{
	check(APIData);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::UpdateLeaderboard);
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::UpdateLeaderboard_Response);
	Request->SetURL(APIUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> PlayerIdJsonArray;

	for (const FString& Username : WinnerUsername)
	{
		PlayerIdJsonArray.Add(MakeShareable(new FJsonValueString(Username)));
	}
	JsonObject->SetArrayField(TEXT("playerIds"), PlayerIdJsonArray);
	FString Content;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	Request->SetContentAsString(Content);
	
	Request->ProcessRequest();
}

void UGameStatsManager::UpdateLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                                   bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("UpdateLeaderboard_Response Failed..."));
	}
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			return;
		}

		UE_LOG(LogDedicatedServers, Warning, TEXT("UpdateLeaderboard_Response Succeeded!"));
		OnUpdatedLeaderboardSucceeded.Broadcast();
	}
}

void UGameStatsManager::RetrieveLeaderboard()
{
	check(APIData);
	RetrieveLeaderboardMessage.Broadcast(TEXT("랭킹 검색 중입니다..."), false);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RetrieveLeaderboard);
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveLeaderboard_Response);
	Request->SetURL(APIUrl);
	Request->SetVerb("GET");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
}

void UGameStatsManager::RetrieveLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		RetrieveLeaderboardMessage.Broadcast(TEXT("검색에 실패했습니다."), false);
		UE_LOG(LogDedicatedServers, Error, TEXT("RetrieveLeaderboard_Response Failed..."));
		return;
	}

	TArray<FDSLeaderboardItem> LeaderboardItems;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			RetrieveLeaderboardMessage.Broadcast(TEXT("검색에 실패했습니다."), false);
			return;
		}
		
		const TArray<TSharedPtr<FJsonValue>>* LeaderboardArray;
		if (JsonObject->TryGetArrayField(TEXT("Leaderboard"), LeaderboardArray))
		{
			for (const TSharedPtr<FJsonValue>& ItemValue : *LeaderboardArray)
			{
				TSharedPtr<FJsonObject> ItemObject = ItemValue->AsObject();
				if (ItemObject.IsValid())
				{
					FDSLeaderboardItem Item;
					if (FJsonObjectConverter::JsonObjectToUStruct(ItemObject.ToSharedRef(), &Item))
					{
						LeaderboardItems.Add(Item);
					}
					else
					{
						UE_LOG(LogDedicatedServers, Error, TEXT("Failed to parse Leaderboard..."));
						return;
					}
				}
			}
		}
	}
	RetrieveLeaderboardMessage.Broadcast(TEXT(""), false);
	OnRetrieveLeaderboard.Broadcast(LeaderboardItems);
}
