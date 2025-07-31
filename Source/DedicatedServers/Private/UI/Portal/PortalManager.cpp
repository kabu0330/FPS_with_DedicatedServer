// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/Interface/HUDManagement.h"
#include "Gameframework/HUD.h"


void UPortalManager::SignIn(const FString& Username, const FString& Password)
{
	SignInStatusMessageDelegate.Broadcast(TEXT("사용자 확인 중..."), false);
	
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, & UPortalManager::SignIn_Response);

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::SignIn);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	LastUserName = Username;
	TMap<FString, FString> Params = {
		{TEXT("username"), Username},
		{TEXT("password"), Password}
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest(); 
}

void UPortalManager::SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		SignInStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			SignInStatusMessageDelegate.Broadcast(TEXT("UserName 또는 Password가 일치하지 않습니다."), true);
			return;
		}

		FDSInitiateAuthResponse InitiateAuthResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &InitiateAuthResponse);
		OnSignInSucceededDelegate.Broadcast();
		InitiateAuthResponse.Dump();

		// LocalPlayerSubsystem에 엑세스 토큰 등과 같은 중요한 데이터를 보관한다.
		// 레벨 이동을 해도 데이터가 고유하게 유지되는 클래스
		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
		if (IsValid(LocalPlayerSubsystem))
		{
			LocalPlayerSubsystem->InitializeTokens(InitiateAuthResponse.AuthenticationResult, this);
			LocalPlayerSubsystem->SetUsername(LastUserName);
			LocalPlayerSubsystem->SetEmail(InitiateAuthResponse.email);
		}

		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			if (IHUDManagement* HUDManagementInterface = Cast<IHUDManagement>(LocalPlayerController->GetHUD()))
			{
				HUDManagementInterface->OnSignIn();
			}
		}
		
	}
}

void UPortalManager::SignUp(const FString& Username, const FString& Password, const FString& Email)
{
	SignUpStatusMessageDelegate.Broadcast(TEXT("새로운 계정을 생성하고 있습니다..."), false);
	
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, & UPortalManager::SignUp_Response);

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::SignUp);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	LastUserName = Username;
	TMap<FString, FString> Params = {
		{TEXT("username"), Username},
		{TEXT("password"), Password},
		{TEXT("email"), Email}
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest(); 
}

void UPortalManager::SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		SignUpStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			if (JsonObject->HasField(TEXT("name")))
			{
				FString Exception = JsonObject->GetStringField(TEXT("name"));
				if (Exception.Equals(TEXT("UsernameExistsException")))
				{
					SignUpStatusMessageDelegate.Broadcast(TEXT("이미 사용 중인 Username입니다. 이름을 변경해주세요."), true);
					return;
				}
			}
			else if (JsonObject->HasField(TEXT("errorType")))
			{
				FString Exception = JsonObject->GetStringField(TEXT("errorType"));
				if (Exception.Contains(TEXT("UsernameExistsException")))
				{
					SignUpStatusMessageDelegate.Broadcast(TEXT("이미 사용 중인 Username입니다. 이름을 변경해주세요."), true);
				}
			}
			SignUpStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
			return;
		}
		
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &LastSignUpResponse);
		OnSignUpSucceededDelegate.Broadcast();
		
		LastSignUpResponse.Dump();
	}
}

void UPortalManager::Confirm(const FString& ConfirmationCode)
{
	SignUpStatusMessageDelegate.Broadcast(TEXT("인증 번호를 확인합니다..."), false);
	
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, & UPortalManager::Confirm_Response);

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::ConfirmSignUp);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("PUT"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));


	TMap<FString, FString> Params = {
		{TEXT("username"), LastUserName},
		{TEXT("confirmationCode"), ConfirmationCode},
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest(); 
}

void UPortalManager::Confirm_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ConfirmStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			if (JsonObject->HasField(TEXT("name")))
			{
				FString Exception = JsonObject->GetStringField(TEXT("name"));
				if (Exception.Equals(TEXT("CodeMismatchException")))
				{
					ConfirmStatusMessageDelegate.Broadcast(TEXT("인증 번호가 일치하지 않습니다."), true);
					return;
				}
			}
			ConfirmStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWentWrong, true);
			return;
		}
	}
	OnConfirmSucceededDelegate.Broadcast();
}

void UPortalManager::QuitGame()
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		UKismetSystemLibrary::QuitGame(this, LocalPlayerController, EQuitPreference::Quit, false);
	}
}

void UPortalManager::RefreshTokens(const FString& RefreshToken)
{
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, & UPortalManager::RefreshTokens_Response);

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::SignIn);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	TMap<FString, FString> Params = {
		{TEXT("refreshToken"), RefreshToken}
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest(); 
}

void UPortalManager::RefreshTokens_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true)) return; 

		FDSInitiateAuthResponse InitiateAuthResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &InitiateAuthResponse);
		
		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
		if (IsValid(LocalPlayerSubsystem))
		{
			LocalPlayerSubsystem->UpdateTokens(
				InitiateAuthResponse.AuthenticationResult.AccessToken,
				InitiateAuthResponse.AuthenticationResult.IdToken
				);
		}
	}
}

void UPortalManager::SignOut(const FString& AccessToken)
{
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, & UPortalManager::SignOut_Response);

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::SignOut);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	TMap<FString, FString> Params = {
		{TEXT("accessToken"), AccessToken}
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest(); 
}

void UPortalManager::SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		return;
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject, true))
		{
			return;
		}
		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		// 부모 포인터가 자식 인스턴스를 가리키고 있다.(업캐스팅)
		// true라면 인터페이스가 제시한 순수 가상함수를 반드시 구현한 상태이니까 부모 포인터로 자식 인스턴스의 함수를 호출할 수 있다.
		if (IHUDManagement* HUDManagementInterface = Cast<IHUDManagement>(LocalPlayerController->GetHUD()))
		{
			HUDManagementInterface->OnSignOut();
		}
	}
}
