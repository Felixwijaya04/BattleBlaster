// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ScreenMessage.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API UScreenMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget)) UTextBlock* MessageTextBlock;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) UTextBlock* LevelNameBlock;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) UTextBlock* EnemyNumTracker;

	void SetMessageText(FString Message);
	void SetLevelName(FString Name);
	void SetEnemiesLeft(FString Number);
};
