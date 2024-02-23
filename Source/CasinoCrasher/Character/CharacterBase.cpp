// Copyright 2024 Thomas Ballantyne. All rights reserved.


#include "CharacterBase.h"

#include "Abilities/CCAbilitySystemComponent.h"
#include "Abilities/CCGameplayAbility.h"
#include "Abilities/AttributeSets/CCAttributeSetBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UCCAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSetBase = CreateDefaultSubobject<UCCAttributeSetBase>(TEXT("AttributeSetBase"));
	
	
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		// Bind to AbilitySystemComponent
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/CasinoCrasher"), FName("ECCAbilityInputID"));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(ECCAbilityInputID::Confirm), static_cast<int32>(ECCAbilityInputID::Cancel)));
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AAAAAAAAAAAAA"));	
	}
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool ACharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ACharacterBase::GetHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float ACharacterBase::GetMaxHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxHealth();
	}
	
	return 0.0f;
}

float ACharacterBase::GetStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStamina();
	}

	return 0.0f;
}

float ACharacterBase::GetMaxStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxStamina();
	}

	return 0.0f;
}

float ACharacterBase::GetMoveSpeed() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeed();
	}

	return 0.0f;
}

float ACharacterBase::GetMoveSpeedBaseValue() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase)->GetBaseValue();
	}

	return 0.0f;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACharacterBase::AddCharacterAbilities()
{

	if (!IsValid(AbilitySystemComponent) || AbilitySystemComponent->bCharacterAbilitiesGiven)
		return;

	for (TSubclassOf<UCCGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void ACharacterBase::InitializeAttributes()
{
	if (!IsValid(AbilitySystemComponent))
		return;
	
	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
	}
}

void ACharacterBase::AddStartupEffects()
{
	if (!IsValid(AbilitySystemComponent) || AbilitySystemComponent->bStartupEffectsApplied)
		return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}