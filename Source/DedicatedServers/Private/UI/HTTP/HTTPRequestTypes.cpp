#include "UI/HTTP/HTTPRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace HTTPStatusMessage
{
	const FString SomethingWentWrong{TEXT("예기치 못한 문제가 발생했습니다.")};
}

void FDSMetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("--- MetaData DUMP---"));
	UE_LOG(LogDedicatedServers, Log, TEXT("httpStatusCode: %d"), httpStatusCode);
	UE_LOG(LogDedicatedServers, Log, TEXT("requestId: %s"), *requestId);
	UE_LOG(LogDedicatedServers, Log, TEXT("attempts: %d"), attempts);
	UE_LOG(LogDedicatedServers, Log, TEXT("totalRetryDelay: %f"), totalRetryDelay);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- END MetaData DUMP---"));
}

void FDSListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("ListFleetsResponse:"));
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	}
	if (!NextToken.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("NextToken: %s"), *NextToken);
	}
}

void FDSGameSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("--- GameSession DUMP---"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_YELLOW);
	UE_LOG(LogDedicatedServers, Log, TEXT("  CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("  CreatorId: %s"), *CreatorId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  CurrentPlayerSessionCount: %d"), CurrentPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("  DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("  FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("  FleetId: %s"), *FleetId);
	for (const auto& Elem : GameProperties)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("  GameProperty: %s = %s"), *Elem.Key, *Elem.Value);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("  GameSessionData: %s"), *GameSessionData);
	UE_LOG(LogDedicatedServers, Log, TEXT("  GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Location: %s"), *Location);
	UE_LOG(LogDedicatedServers, Log, TEXT("  MatchmakerData: %s"), *MatchmakerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("  MaximumPlayerSessionCount: %d"), MaximumPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Name: %s"), *Name);
	UE_LOG(LogDedicatedServers, Log, TEXT("  PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("  StatusReason: %s"), *StatusReason);
	UE_LOG(LogDedicatedServers, Log, TEXT("  TerminationTime: %s"), *TerminationTime);
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- END GameSession DUMP---"));
}
void FDSPlayerSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("--- FDSPlayerSession DUMP ---"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_YELLOW);
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerData: %s"), *PlayerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerId: %s"), *PlayerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionId: %s"), *PlayerSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime: %s"), *TerminationTime);
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- END FDSPlayerSession DUMP ---"));
}

void FDSCodeDeliveryDetails::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("AttributeName: %s"), *AttributeName);
	UE_LOG(LogDedicatedServers, Log, TEXT("DeliveryMedium: %s"), *DeliveryMedium);
	UE_LOG(LogDedicatedServers, Log, TEXT("Destination: %s"), *Destination);
}

void FDSSignUpResponse::Dump() const
{
	UE_LOG(LogTemp, Warning, TEXT("--- FDSSignUpResponse DUMP ---"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_YELLOW);
	CodeDeliveryDetails.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: %s"), UserConfirmed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogDedicatedServers, Log, TEXT("UserSub: %s"), *UserSub);
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
	UE_LOG(LogTemp, Warning, TEXT("--- END FDSSignUpResponse DUMP ---"));
}

void FDSNewDeviceMetadata::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("  --- NewDeviceMetadata ---"));
	UE_LOG(LogDedicatedServers, Log, TEXT("    DeviceGroupKey: %s"), *DeviceGroupKey);
	UE_LOG(LogDedicatedServers, Log, TEXT("    DeviceKey: %s"), *DeviceKey);
}

void FDSAuthenticationResult::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("  --- AuthenticationResult ---"));
	UE_LOG(LogDedicatedServers, Log, TEXT("    AccessToken: %s"), *AccessToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("    ExpiresIn: %s"), *ExpiresIn);
	UE_LOG(LogDedicatedServers, Log, TEXT("    IdToken: %s"), *IdToken);
	NewDeviceMetadata.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("    RefreshToken: %s"), *RefreshToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("    TokenType: %s"), *TokenType);
}

void FDSInitiateAuthResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("--- FDSSignInResponse DUMP ---"));
	AuthenticationResult.Dump();

	UE_LOG(LogDedicatedServers, Log, TEXT("  AvailableChallenges:"));
	for (const FString& Challenge : AvailableChallenges)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("    - %s"), *Challenge);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("  ChallengeName: %s"), *ChallengeName);

	UE_LOG(LogDedicatedServers, Log, TEXT("  ChallengeParameters:"));
	for (const auto& Elem : ChallengeParameters)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("    %s: %s"), *Elem.Key, *Elem.Value);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("  Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("  email: %s"), *email);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- END FDSSignInResponse DUMP ---"));
}