// Copyright 2024 Thomas Ballantyne. All rights reserved.


#include "../Player/CCPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h" 

#include "PlayerInputInterface.h"

void ACCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ACCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACCPlayerController::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACCPlayerController::Look);
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACCPlayerController::Jump);
		
		//Start Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACCPlayerController::StartSprint);
		
		//Stop Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACCPlayerController::StopSprint);
		
		//Attack 1
		EnhancedInputComponent->BindAction(Attack1Action, ETriggerEvent::Triggered, this, &ACCPlayerController::Attack1);
		
		//Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ACCPlayerController::Roll);
	}
}

void ACCPlayerController::Move(const FInputActionValue& Value)
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Move(pawn, Value);
	}
}

void ACCPlayerController::Look(const FInputActionValue& Value)
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Look(pawn, Value);
	}
}

void ACCPlayerController::Jump()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Jump(pawn);
	}
}

void ACCPlayerController::StartSprint()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_StartSprint(pawn);
	}
}

void ACCPlayerController::StopSprint()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_StopSprint(pawn);
	}
}

void ACCPlayerController::Attack1()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Attack1(pawn);
	}
}

void ACCPlayerController::Attack2()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Attack2(pawn);
	}
}

void ACCPlayerController::Attack3()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Attack3(pawn);
	}
}

void ACCPlayerController::Interact()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Interact(pawn);
	}
}

void ACCPlayerController::Roll()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Roll(pawn);
	}
}
