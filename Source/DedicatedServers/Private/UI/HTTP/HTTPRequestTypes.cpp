#include "UI/HTTP/HTTPRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace HTTPStatusMessage
{
	const FString SomethingWentWrong{TEXT("알 수 없는 문제입니다.")};
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
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_PURPLE);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- GameSession DUMP---"));
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
	UE_LOG(LogDedicatedServers, Log, TEXT("--- END GameSession DUMP---"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
}
void FDSPlayerSession::Dump() const
{
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_PURPLE);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- FDSPlayerSession DUMP ---"));
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
	UE_LOG(LogDedicatedServers, Log, TEXT("--- END FDSPlayerSession DUMP ---"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
}

void FDSCodeDeliveryDetails::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("AttributeName: %s"), *AttributeName);
	UE_LOG(LogDedicatedServers, Log, TEXT("DeliveryMedium: %s"), *DeliveryMedium);
	UE_LOG(LogDedicatedServers, Log, TEXT("Destination: %s"), *Destination);
}

void FDSSignUpResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_PURPLE);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- FDSSignUpResponse DUMP ---"));
	CodeDeliveryDetails.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: %s"), UserConfirmed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogDedicatedServers, Log, TEXT("UserSub: %s"), *UserSub);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- END FDSSignUpResponse DUMP ---"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
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
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_PURPLE);
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
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
}

void FDSMatchStats::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("  scoredElims: %d"), scoredElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  defeats: %d"), defeats);
	UE_LOG(LogDedicatedServers, Log, TEXT("  hits: %d"), hits);
	UE_LOG(LogDedicatedServers, Log, TEXT("  misses: %d"), misses);
	UE_LOG(LogDedicatedServers, Log, TEXT("  headShootElims: %d"), headShootElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  highestElims: %d"), highestElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  hightestStreak: %d"), hightestStreak);
	UE_LOG(LogDedicatedServers, Log, TEXT("  dethroneElims: %d"), dethroneElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  revengeElims: %d"), revengeElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  showStopperElims: %d"), showStopperElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  gotFirstBlood: %d"), gotFirstBlood);
	UE_LOG(LogDedicatedServers, Log, TEXT("  matchWins: %d"), matchWins);
	UE_LOG(LogDedicatedServers, Log, TEXT("  matchLosses: %d"), matchLosses);

}

void FDSRecordMatchStatsInput::Dump() const
{
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_PURPLE);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- FDSRecordMatchStatsInput Dump ---"));
	UE_LOG(LogDedicatedServers, Log, TEXT("  username: %s"), *username);
	matchStats.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("----------------------------------------"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
}

void FDSRetrieveMatchStatsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_PURPLE);
	UE_LOG(LogDedicatedServers, Log, TEXT("--- FDSRetrieveMatchStatsResponse Dump ---"));
	UE_LOG(LogDedicatedServers, Log, TEXT("Username: %s"), *username);
	UE_LOG(LogDedicatedServers, Log, TEXT("Email: %s"), *email);
	UE_LOG(LogDedicatedServers, Log, TEXT("Database ID: %s"), *databaseid);
	UE_LOG(LogDedicatedServers, Log, TEXT("Scored Elims: %d"), scoredElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("Highest Elims: %d"), highestElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("Dethrone Elims: %d"), dethroneElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("Defeats: %d"), defeats);
	UE_LOG(LogDedicatedServers, Log, TEXT("Hits: %d"), hits);
	UE_LOG(LogDedicatedServers, Log, TEXT("Misses: %d"), misses);
	UE_LOG(LogDedicatedServers, Log, TEXT("Headshot Elims: %d"), headShootElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("Highest Streak: %d"), hightestStreak);
	UE_LOG(LogDedicatedServers, Log, TEXT("Revenge Elims: %d"), revengeElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("Show Stopper Elims: %d"), showStopperElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("Got First Blood: %d"), gotFirstBlood);
	UE_LOG(LogDedicatedServers, Log, TEXT("Match Wins: %d"), matchWins);
	UE_LOG(LogDedicatedServers, Log, TEXT("Match Losses: %d"), matchLosses);
	UE_LOG(LogDedicatedServers, Log, TEXT("----------------------------------------"));
	UE_LOG(LogDedicatedServers, SetColor, TEXT("%s"), COLOR_NONE);
}
