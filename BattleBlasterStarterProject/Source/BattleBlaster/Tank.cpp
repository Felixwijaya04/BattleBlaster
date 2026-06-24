// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArmComponent"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);	// CapsuleComponent is declared in BasePawn.h
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	// The code below is "boilerplate", you don't need to fully understand of it
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	SetPlayerEnabled(false);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.0f, 4, FColor::Red);
		RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::RotateInput);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ATank::Fire);
	}
}

void ATank::MoveInput(const FInputActionValue& value)
{
	float InputValue = value.Get<float>();

	FVector DeltaLocation = FVector(0, 0, 0);
	DeltaLocation.X = MoveSpeed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::RotateInput(const FInputActionValue& value)
{
	float InputValue = value.Get<float>();

	FRotator DeltaRotation = FRotator(0, 0, 0);
	DeltaRotation.Yaw = RotateSpeed * InputValue * GetWorld()->GetDeltaSeconds();
	
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabled(false);
}

void ATank::SetPlayerEnabled(bool enabled)
{
	if (PlayerController)
	{
		if (enabled)
		{
			IsAlive = true;
			EnableInput(PlayerController);
		}
		else
		{
			IsAlive = false;
			DisableInput(PlayerController);
		}
		PlayerController->bShowMouseCursor = enabled;
	}
}
