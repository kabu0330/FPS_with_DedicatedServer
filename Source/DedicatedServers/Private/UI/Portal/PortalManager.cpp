// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UPortalManager::JoinGameSession()
{
	BroadcastJoinGameSessionMessage.Broadcast(TEXT("Searching for Game Session..."), false);
	
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, & UPortalManager::FindOrCreateGameSession_Response);

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest(); // Queue에 작업 추가 => 워커 스레드가 작업
}

void UPortalManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Find Or Create Game Session Response Received");

	if (!bWasSuccessful)
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
			return;
		}
		
		DumpMetaData(JsonObject);

		// 실제 변환에 사용할 게임 세션 JSON 객체를 담을 변수
		TSharedPtr<FJsonObject> GameSessionJsonObject = JsonObject;

		// 기존 게임 세션이 존재할 때 Json 데이터와 신규 생성 Json Data의 형식 차이 문제
		// 만약 최상위 객체에 "GameSession"이라는 필드가 있다면
		// 한 단계 안으로 들어가서 실제 데이터 객체를 가져와야 한다.
		TSharedPtr<FJsonValue> GameSessionValue;
		if (GameSessionValue = JsonObject->TryGetField(TEXT("GameSession")))
		{
			GameSessionJsonObject = GameSessionValue->AsObject();
		}

		if (GameSessionJsonObject.IsValid())
		{
			FDSGameSession GameSession;
			FJsonObjectConverter::JsonObjectToUStruct(GameSessionJsonObject.ToSharedRef(), &GameSession);
			GameSession.Dump();
			BroadcastJoinGameSessionMessage.Broadcast(TEXT("Found Game Session"), false);
		}
		else
		{
			BroadcastJoinGameSessionMessage.Broadcast(TEXT("Game Session Data is lost"), true);
		}
		
	}
}
