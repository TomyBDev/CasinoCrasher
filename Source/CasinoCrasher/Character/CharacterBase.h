// Copyright 2024 Thomas Ballantyne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UCCAbilitySystemComponent;
class UCCAttributeSetBase;
class UCCGameplayAbility;

UCLASS()
class CASINOCRASHER_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter")
	virtual bool IsAlive() const;

	/**
	* Getters for attributes from GSAttributeSetBase
	**/

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxStamina() const;

	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMoveSpeedBaseValue() const;

protected:

	UPROPERTY()
	UCCAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	UCCAttributeSetBase* AttributeSetBase;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UCCGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();
};
