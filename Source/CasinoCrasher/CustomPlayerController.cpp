// Copyright 2024 Thomas Ballantyne. All rights reserved.


#include "CustomPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h" 

#include "PlayerInputInterface.h"

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Look);
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Jump);
		
		//Start Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::StartSprint);
		
		//Stop Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACustomPlayerController::StopSprint);
		
		//Jumping
		EnhancedInputComponent->BindAction(Attack1Action, ETriggerEvent::Triggered, this, &ACustomPlayerController::Attack1);
	}
}

void ACustomPlayerController::Move(const FInputActionValue& Value)
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Move(pawn, Value);
	}
}

void ACustomPlayerController::Look(const FInputActionValue& Value)
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Look(pawn, Value);
	}
}

void ACustomPlayerController::Jump()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Jump(pawn);
	}
}

void ACustomPlayerController::StartSprint()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_StartSprint(pawn);
	}
}

void ACustomPlayerController::StopSprint()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_StopSprint(pawn);
	}
}

void ACustomPlayerController::Attack1()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Attack1(pawn);
	}
}

void ACustomPlayerController::Attack2()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Attack2(pawn);
	}
}

void ACustomPlayerController::Attack3()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Attack3(pawn);
	}
}

void ACustomPlayerController::Interact()
{
	APawn* pawn = GetPawn();
	if (UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass())) {				
		IPlayerInputInterface::Execute_Interact(pawn);
	}
}
