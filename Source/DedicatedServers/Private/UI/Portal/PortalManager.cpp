// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "aws/gamelift/server/model/GameSession.h"
#include "aws/gamelift/server/model/GameSessionStatus.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Kismet/GameplayStatics.h"
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

	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_DARK_BLUE);
	UE_LOG(LogDedicatedServers, Log, TEXT("1. PortalManager send an HTTP Request to FindOrCreateGameSession AWS Lambda function"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
}

void UPortalManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_DARK_BLUE);
	UE_LOG(LogDedicatedServers, Log, TEXT("2. PortalManager received an HTTP Response from FindOrCreateGameSession AWS Lambda function"));

	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("2. HTTP Response is Failed"));
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			UE_LOG(LogDedicatedServers, Log, TEXT("2. HTTP Response is Error Message"));
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
			return;
		}
		
		DumpMetaData(JsonObject);

		UE_LOG(LogDedicatedServers, Log, TEXT("2. Parsing the GameSession..."));
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
			UE_LOG(LogDedicatedServers, Log, TEXT("3. Successfully parsed the GameSession!!!"));
			FDSGameSession GameSession;
			FJsonObjectConverter::JsonObjectToUStruct(GameSessionJsonObject.ToSharedRef(), &GameSession);

			const FString GameSessionId = GameSession.GameSessionId;
			const FString GameSessionStatus = GameSession.Status;
			HandleGameSessionStatus(GameSessionStatus, GameSessionId);
		}
		else
		{
			UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_RED);
			UE_LOG(LogDedicatedServers, Log, TEXT("3. Failed to parse the GameSession!!!"));
			BroadcastJoinGameSessionMessage.Broadcast(TEXT("Game Session Data is lost"), true);
		}
		
	}

	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
}

void UPortalManager::HandleGameSessionStatus(const FString& Status, const FString& SessionId)
{
	UE_LOG(LogDedicatedServers, Log, TEXT("4. Parsing the GameSession Status..."));
	if (Status.Equals(TEXT("ACTIVE")))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("4. Found activate Game Session Creating a Player Session..."));
		BroadcastJoinGameSessionMessage.Broadcast(TEXT("Found activate Game Session. Creating a Player Session..."), false);
		
		TryCreatePlayerSession(GetUniquePlayerId(), SessionId);
	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("4. GameSession Status is ACTIVATING, therefore retrying JoinGameSession after a delay..."));
		FTimerDelegate CreateSessionDelegate;
		CreateSessionDelegate.BindUObject(this, &ThisClass::JoinGameSession);
		
		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			LocalPlayerController->GetWorldTimerManager().SetTimer(CreateSessionTimer, CreateSessionDelegate, 0.5f, false);
			return;
		}
		UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_RED);
		UE_LOG(LogDedicatedServers, Log, TEXT("4. GameSession Status is ACTIVATING, but APlayerController is nullptr"));
	}
	else
	{
		UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_RED);
		UE_LOG(LogDedicatedServers, Log, TEXT("4. GameSession Status is not ACTIVE or ACTIVATING"));
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
	}
}

FString UPortalManager::GetUniquePlayerId() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("5. Parsing the Player Id of GameSession..."));
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		APlayerState* LocalPlayerState = LocalPlayerController->GetPlayerState<APlayerState>();
		if (IsValid(LocalPlayerState) && LocalPlayerState->GetUniqueId().IsValid())
		{
			UE_LOG(LogDedicatedServers, Log, TEXT("5. Set Unique Player Id"));
			return TEXT("Player_") + FString::FromInt(LocalPlayerState->GetUniqueID());
		}
	}
	
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_RED);
	UE_LOG(LogDedicatedServers, Log, TEXT("5. Failed to Unique Player Id"));
	return FString();
}

void UPortalManager::TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId)
{
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, & UPortalManager::CreatePlayerSession_Response);

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::CreatePlayerSession);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Lambda로 event 인자에 들어갈 gameSessionId, playerId를 Json에 담아 함께 보내야 한다.
	TMap<FString, FString> Params = {
		{TEXT("playerId"), PlayerId},
		{TEXT("gameSessionId"), GameSessionId}
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest(); // Queue에 작업 추가 => 워커 스레드가 작업

	UE_LOG(LogDedicatedServers, Log, TEXT("6. PortalManager send an HTTP Request to CreatePlayerSession AWS Lambda function"));
}

void UPortalManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	UE_LOG(LogDedicatedServers, Log, TEXT("7. PortalManager received an HTTP Response from CreatePlayerSession AWS Lambda function"));
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			UE_LOG(LogDedicatedServers, Log, TEXT("8. HTTP Response is Error Message"));
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
			return;
		}
		UE_LOG(LogDedicatedServers, Log, TEXT("8. Parsing the Player Session..."));
		
		FDSPlayerSession PlayerSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerSession);
		PlayerSession.Dump();

		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			FInputModeGameOnly InputModeData;
			LocalPlayerController->SetInputMode(InputModeData);
			LocalPlayerController->SetShowMouseCursor(false);
		}

		const FString IpAndPort = PlayerSession.IpAddress + TEXT(":") + FString::FromInt(PlayerSession.Port);
		const FName Address(*IpAndPort);
		UGameplayStatics::OpenLevel(this, Address);
		
		UE_LOG(LogDedicatedServers, Log, TEXT("9. Open Level, %s"), *IpAndPort);
	}
}
