// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameModeBase.h"

#include "Player/DSPlayerController.h"

void ADS_GameModeBase::StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	/* Bind를 Weak로 설정한 이유
	 * 타이머 종료 후 또는 타이머 종료 전에 게임 모드 객체가 먼저 파괴될 수 있다.
	 * (시간차 때문에 서버 트래블이 먼저 일어난 뒤 타이머가 호출되거나, 레벨이 종료되는 경우)
	 * 이 때, BindWeak로 설정하면 객체가 파괴되면 nullptr를 반환하면서 람다 함수를 더 이상 호출하지 않게 된다.
	 * 메모리 크러시 방지
	 */ 
	
	// 종료 타이머가 호출될 때 실행할 함수
	CountdownTimerHandle.TimerFinishedDelegate.BindWeakLambda(this, [&]()
	{
		StopCountdownTimer(CountdownTimerHandle);
		OnCountdownTimerFinished(CountdownTimerHandle.Type);
	});

	// 종료 타이머 설정
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle.TimerFinishedHandle,
		CountdownTimerHandle.TimerFinishedDelegate,
		CountdownTimerHandle.CountdownTime,
		false
		);

	// Interval 초마다 호출될 함수
	// 모든 클라이언트에게 남은 시간을 전파.
	CountdownTimerHandle.TimerUpdateDelegate.BindWeakLambda(this, [&]()
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get());
			if (IsValid(DSPlayerController))
			{
				const float CountdownTimeLeft =
					CountdownTimerHandle.CountdownTime - GetWorldTimerManager().GetTimerRemaining(CountdownTimerHandle.TimerFinishedHandle);
				// 서버 : 얘들아, 지금 4초 남았어!
				DSPlayerController->Client_TimerUpdated(CountdownTimeLeft, CountdownTimerHandle.Type);
				// 클라 : 서버가 0.2초 전에 얘기했으니까 지금은 3.8초 남았네. 델리게이트야 3.8초로 전파해.
			}
		}
	});

	// Interval 시간마다 다시 타이머 업데이트
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle.TimerUpdateHandle,
		CountdownTimerHandle.TimerUpdateDelegate,
		CountdownTimerHandle.CountdownUpdateInterval,
		true
		);
}

void ADS_GameModeBase::StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	CountdownTimerHandle.State = ECountdownTimerState::Stopped;
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerFinishedHandle);
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerUpdateHandle);
	if (CountdownTimerHandle.TimerFinishedDelegate.IsBound())
	{
		CountdownTimerHandle.TimerFinishedDelegate.Unbind();
	}
	if (CountdownTimerHandle.TimerUpdateDelegate.IsBound())
	{
		CountdownTimerHandle.TimerUpdateDelegate.Unbind();
	}

	// 모든 클라이언트에게 타이머를 종료하라고 알린다.
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get());
		if (IsValid(DSPlayerController))
		{
			// 서버 : 얘들아 시간 다 됐어. 카운트다운 Stop이야!
			DSPlayerController->Client_TimerStopped(0.f, CountdownTimerHandle.Type );
		}
	}
}

void ADS_GameModeBase::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	
}
