// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
	FText TextMessage = FText::FromString(Message);
	MessageTextBlock->SetText(TextMessage);
}

void UScreenMessage::SetLevelName(FString Name)
{
	FText TextMessage = FText::FromString(Name);
	LevelNameBlock->SetText(TextMessage);
}

void UScreenMessage::SetEnemiesLeft(FString Number)
{
	FText TextMessage = FText::FromString(Number);
	EnemyNumTracker->SetText(TextMessage);
}
