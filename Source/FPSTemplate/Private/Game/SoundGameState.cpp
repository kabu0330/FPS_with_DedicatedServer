// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SoundGameState.h"

#include <random>

#include "Kismet/GameplayStatics.h"

void ASoundGameState::Multicast_PlaySoundOnFirstBlood_Implementation()
{
	check(IsValid(GetWorld()));
	if (!IsValid(FirstBloodSound)) return;

	UGameplayStatics::PlaySound2D(GetWorld(), FirstBloodSound);
}

void ASoundGameState::Server_PlaySoundOnFirstBlood_Implementation()
{
	Multicast_PlaySoundOnFirstBlood();
}

void ASoundGameState::Server_PlaySoundOnMultiKill_Implementation(int32 MultiKillCount)
{
	Multicast_PlaySoundOnMultiKill(MultiKillCount);
}

void ASoundGameState::Multicast_PlaySoundOnMultiKill_Implementation(int32 MultiKillCount)
{
	check(IsValid(GetWorld()));

	switch (MultiKillCount)
	{
	case 2:
		check(IsValid(DoubleKillSound));
		UGameplayStatics::PlaySound2D(GetWorld(), DoubleKillSound);	
		break;
	case 3:
		check(IsValid(TripleKillSound));
		UGameplayStatics::PlaySound2D(GetWorld(), TripleKillSound);	
		break;
	case 4:
		check(IsValid(QuadKillSound));
		UGameplayStatics::PlaySound2D(GetWorld(), QuadKillSound);	
		break;
	case 5:
	default:
		check(IsValid(PentaKillSound));
		UGameplayStatics::PlaySound2D(GetWorld(), PentaKillSound);	
		break;
	case 1:
		break;
	}
}



