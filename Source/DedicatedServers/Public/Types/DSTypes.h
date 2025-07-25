#pragma once

#include "CoreMinimal.h"
#include "DSTypes.generated.h"

UENUM(BlueprintType)
enum class ECountdownTimerState : uint8
{
	NotStarted	 UMETA(DisplayName = "준비"),
	Started		 UMETA(DisplayName = "시작"),
	Paused		 UMETA(DisplayName = "일시정지"),
	Stopped		 UMETA(DisplayName = "중단")
};

UENUM(BlueprintType)
enum class ECountdownTimerType : uint8
{
	LobbyCountdown		UMETA(DisplayName = "로비 카운트다운"),
	PreMatch			UMETA(DisplayName = "경기 전"),
	Match				UMETA(DisplayName = "경기 중"),
	PostMatch			UMETA(DisplayName = "경기 후"),
	None				UMETA(DisplayName = "NONE"),
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

UENUM()
enum class ELobbyStatus : uint8
{
	WaitingForPlayers,
	CountdownToSeamlessTravel,
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