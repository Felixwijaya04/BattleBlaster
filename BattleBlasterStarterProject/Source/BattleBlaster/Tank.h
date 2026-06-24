// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

#include "Tank.generated.h"


/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATank();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="Input") class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction* TurnAction;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction* FireAction;
	
	UPROPERTY(VisibleAnywhere) USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere) class UCameraComponent* CameraComponent; //adding class keyword is forward declaring method to increase performance and type the header file in the .cpp file
	UPROPERTY(VisibleAnywhere) bool IsAlive = true;
	
	UPROPERTY(EditAnywhere) float MoveSpeed = 600;
	UPROPERTY(EditAnywhere) float RotateSpeed = 100;
	
	void MoveInput(const FInputActionValue& value);
	void RotateInput(const FInputActionValue& value);
	void HandleDestruction();
	void SetPlayerEnabled(bool enabled);

	APlayerController* PlayerController;
};
