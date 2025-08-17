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

void ASoundGameState::PlaySoundOnKill()
{
	check(IsValid(GetWorld()));

	std::random_device Random;
	std::mt19937 RandomEngine(Random());
	std::uniform_int_distribution<int> RandomDistribution(1, 4);
	int RandomNumber = RandomDistribution(RandomEngine);
	UE_LOG(LogTemp, Warning, TEXT("PlaySoundOnKill RandomNumber: %d"), RandomNumber);
	
	switch (RandomNumber)
	{
	case 1:
	default:
		if (!IsValid(KillSound)) return;
		UGameplayStatics::PlaySound2D(GetWorld(), KillSound);
		break;
	case 2:
		if (!IsValid(KillSound2)) return;
		UGameplayStatics::PlaySound2D(GetWorld(), KillSound2);
		break;
	case 3:
		if (!IsValid(KillSound3)) return;
		UGameplayStatics::PlaySound2D(GetWorld(), KillSound3);
		break;
	case 4:
		if (!IsValid(KillSound4)) return;
		UGameplayStatics::PlaySound2D(GetWorld(), KillSound4);
		break;
	}
}

