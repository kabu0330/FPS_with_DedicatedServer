#include "UI/HTTP/HTTPRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

void FDSMetaData::Dump() const
{
	UE_LOG(Log_DedicatedServer, Log, TEXT("MetaData:"));
	UE_LOG(Log_DedicatedServer, Log, TEXT("httpStatusCode: %d"), httpStatusCode);
	UE_LOG(Log_DedicatedServer, Log, TEXT("requestId: %s"), *requestId);
	UE_LOG(Log_DedicatedServer, Log, TEXT("attempts: %d"), attempts);
	UE_LOG(Log_DedicatedServer, Log, TEXT("totalRetryDelay: %f"), totalRetryDelay);
	
}

void FDSListFleetsResponse::Dump() const
{
	UE_LOG(Log_DedicatedServer, Log, TEXT("ListFleetsResponse:"));
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(Log_DedicatedServer, Log, TEXT("FleetId: %s"), *FleetId);
	}
	if (!NextToken.IsEmpty())
	{
		UE_LOG(Log_DedicatedServer, Log, TEXT("NextToken: %s"), *NextToken);
	}
}
