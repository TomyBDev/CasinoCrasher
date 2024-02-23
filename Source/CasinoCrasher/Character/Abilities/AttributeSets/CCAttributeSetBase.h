// Copyright 2024 Thomas Ballantyne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CCAttributeSetBase.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class CASINOCRASHER_API UCCAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCCAttributeSetBase();

	// Current Health, when 0 we expect owner to die unless prevented by an ability. Capped by MaxHealth.
	// Positive changes can directly use this.
	// Negative changes to Health should go through Damage meta attribute.
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCCAttributeSetBase, Health)

	// MaxHealth is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCCAttributeSetBase, MaxHealth)

	// Health regen rate will passively increase Health every second
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UCCAttributeSetBase, HealthRegenRate)

	// Current stamina, used to execute special abilities. Capped by MaxStamina.
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCCAttributeSetBase, Stamina)

	// MaxStamina is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCCAttributeSetBase, MaxStamina)

	// Stamina regen rate will passively increase Stamina every second
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UCCAttributeSetBase, StaminaRegenRate)

	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCCAttributeSetBase, Damage)

	// MoveSpeed affects how fast characters can move.
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UCCAttributeSetBase, MoveSpeed)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
	FGameplayTag HeadShotTag;

	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
};
