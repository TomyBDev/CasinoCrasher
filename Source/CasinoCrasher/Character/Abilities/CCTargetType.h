// Copyright 2024 Thomas Ballantyne. All rights reserved.

#pragma once

#include "CCTargetType.generated.h"

class AGSCharacterBase;
class AActor;
struct FGameplayEventData;

/**
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class CASINOCRASHER_API UCCTargetType : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UCCTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
	virtual void GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class CASINOCRASHER_API UCCTargetType_UseOwner : public UCCTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UCCTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class CASINOCRASHER_API UCCTargetType_UseEventData : public UCCTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UCCTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
