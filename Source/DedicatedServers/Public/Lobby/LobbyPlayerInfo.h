#pragma once

#include "Net/Serialization/FastArraySerializer.h"
#include "LobbyPlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FLobbyPlayerInfo : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FLobbyPlayerInfo() {}
	FLobbyPlayerInfo(const FString& Name) : Username(Name) {}
	
	UPROPERTY(BlueprintReadWrite)
	FString Username{};
};
/** Fast TArray Serializer
 *  1. 언리얼 엔진에서 제공하는 고성능 직렬화 매커니즘
 *  2. 각 원소에 고유 ID와 버전 번호를 부여
 *  3. 서버에서 특정 원소가 변경되면 원소의 버전 번호를 갱신. 클라는 버전 번호가 서버와 다르면 갱신
 *  4. 순서를 보장하지 않음
 */
USTRUCT()
struct FLobbyPlayerInfoArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FLobbyPlayerInfo> Items;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FLobbyPlayerInfo, FLobbyPlayerInfoArray>(Items, DeltaParams, *this);
	}
	void AddPlayer(const FLobbyPlayerInfo& NewPlayerInfo);
	void RemovePlayer(const FString& Username);
};

template<>
struct TStructOpsTypeTraits<FLobbyPlayerInfoArray> : public TStructOpsTypeTraitsBase2<FLobbyPlayerInfoArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};