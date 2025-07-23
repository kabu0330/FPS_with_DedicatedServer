#pragma once

#include "DSTypes.generated.h"

UENUM(BlueprintType)
enum class ECountdownTimerState : uint8
{
	NotStarted	 UMETA(DisplayName = TEXT("준비")),
	Started		 UMETA(DisplayName = TEXT("시작")),
	Paused		 UMETA(DisplayName = TEXT("일시정지")),
	Stopped		 UMETA(DisplayName = TEXT("중단"))
};

UENUM(BlueprintType)
enum class ECountdownTimerType : uint8
{
	LobbyCountdown		UMETA(DisplayName = TEXT("로비 카운트다운")),
	PreMatch			UMETA(DisplayName = TEXT("매칭 전")),
	Match				UMETA(DisplayName = TEXT("매칭")),
	PostMatch			UMETA(DisplayName = TEXT("매칭 후")),
	None				UMETA(DisplayName = TEXT("NONE")),
};

UENUM()
enum class EMatchStatus : uint8
{
	WaitingForPlayers,
	PreMatch,
	Match,
	PostMatch,
	SeamlessTravelling
};

USTRUCT(BlueprintType)
struct FCountdownTimerHandle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ECountdownTimerState State = ECountdownTimerState::NotStarted;

	UPROPERTY(BlueprintReadWrite)
	ECountdownTimerType Type = ECountdownTimerType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CountdownTime = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CountdownUpdateInterval = 1.0f;

	FTimerHandle TimerFinishedHandle{};
	FTimerHandle TimerUpdateHandle{};
	FTimerDelegate TimerFinishedDelegate{};
	FTimerDelegate TimerUpdateDelegate{};
	
};

inline bool operator==(const FCountdownTimerHandle& Left, const FCountdownTimerHandle& Right)
{
	return Left.Type == Right.Type;
}