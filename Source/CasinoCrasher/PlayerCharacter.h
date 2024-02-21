// Copyright 2024 Thomas Ballantyne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PlayerInputInterface.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class CASINOCRASHER_API APlayerCharacter : public ACharacter, public IPlayerInputInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Move_Implementation(const FInputActionValue& Value) override;

	virtual void Look_Implementation(const FInputActionValue& Value) override;

	virtual void Jump_Implementation() override;

	virtual void StartSprint_Implementation() override;

	virtual void StopSprint_Implementation() override;
	
	virtual void Attack1_Implementation() override;
	
	virtual void Attack2_Implementation() override;
	
	virtual void Attack3_Implementation() override;

	virtual void Interact_Implementation() override;

	virtual void Roll_Implementation() override;

	virtual void Falling() override;

	virtual void Landed(const FHitResult& Hit) override;
	
	virtual void OnJumped_Implementation() override;

private:

	void EndRoll();

	void StartActionLock();
	
	void EndActionLock();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* sprintArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* camera;

	/** Punch Montage */
	UPROPERTY(EditAnywhere, Category = Animations, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* punchMontage;

	/** Active Gameplay Tags */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tags, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer activeGameplayTags;

	FTimerHandle rollTH;
	float rollTime = 1.4625f;

	FTimerHandle actionLockTH;
	float actionLockTime = 0.25f;
};
