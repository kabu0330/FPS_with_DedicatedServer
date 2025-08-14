// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerBox.generated.h"

class ADS_GameState;
class ALobbyState;
struct FLobbyPlayerInfo;
class UPlayerLabel;
class UScrollBox;
/** 1. Lobby에서 현재 서버에 접속한 사용자 이름을 출력할 스크롤 박스 위젯
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULobbyPlayerBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_PlayerInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerLabel> PlayerLabelClass;

protected:
	virtual void NativeOnInitialized() override;

private:
	UPlayerLabel* FindPlayerLabel(const FString& Username);

	UFUNCTION()
	void UpdatePlayerList();
};
