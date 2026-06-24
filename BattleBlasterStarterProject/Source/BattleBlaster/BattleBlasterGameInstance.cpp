// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}

void UBattleBlasterGameInstance::LoadNextLevel()
{
	if (CurrentLevelIndex < LastLevelIndex)
	{
		ChangeLevel(++CurrentLevelIndex);
	}
	else
	{
		RestartGame();	
	}
}

void UBattleBlasterGameInstance::RestartGame()
{
	ChangeLevel(1);
}

void UBattleBlasterGameInstance::ChangeLevel(int32 LevelIndex)
{
	if (LevelIndex > 0 && LevelIndex <= LastLevelIndex)
	{
		CurrentLevelIndex = LevelIndex;
		UE_LOG(LogTemp, Warning, TEXT("Level Index: %d"), CurrentLevelIndex);
		FString LevelNameString = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
		UGameplayStatics::OpenLevel(GetWorld(), *LevelNameString);
	}
}
