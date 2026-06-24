// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*>Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display,TEXT("Number of Towers: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp,Warning,TEXT("No Tank"));
		}
	}
	// Setting every tower in game targets to tank player 
	int32 index = 0;
	while (index < TowerCount)
	{
		AActor* TowerActor = Towers[index];
		ATower* Tower = Cast<ATower>(TowerActor);
		if (Tower && Tank)
		{
			Tower->Tank = Tank;
		}
		index++;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready!");
			ScreenMessageWidget->SetEnemiesLeft(FString::FromInt(TowerCount) + " enemies left");
			UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GetGameInstance());
			if (BattleBlasterGameInstance)
			{
				ScreenMessageWidget->SetLevelName("Level: " + FString::FromInt(BattleBlasterGameInstance->CurrentLevelIndex));
			}
		}
	}
	
	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1, true);
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds--;
	if (CountdownSeconds > 0)
	{
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		ScreenMessageWidget->SetMessageText("Launch Mission!");
		Tank->SetPlayerEnabled(true);
		//UE_LOG(LogTemp,Display,TEXT("Timer Stops"));
	}
	else
	{
		//ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
		ScreenMessageWidget->MessageTextBlock->SetVisibility(ESlateVisibility::Hidden);
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* Actor)
{
	bool isGameOver = false;
	
	if (Actor == Tank)
	{
		Tank->HandleDestruction();
		isGameOver = true;
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(Actor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();
			TowerCount--;
			ScreenMessageWidget->SetEnemiesLeft(FString::FromInt(TowerCount) + " enemies left");
			if (TowerCount <= 0)
			{
				isGameOver = true;
				isVictory = true;
			}
		}
	}

	if (isGameOver)
	{
		FString GameOverString = isVictory ? "Victory" : "GameOver";
		ScreenMessageWidget->SetMessageText(GameOverString);
		ScreenMessageWidget->MessageTextBlock->SetVisibility(ESlateVisibility::Visible);
		
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer,this , &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay,false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (isVictory)
			{
				UE_LOG(LogTemp,Display,TEXT("Next Level"));
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				UE_LOG(LogTemp,Display,TEXT("Restart Level"));
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}

