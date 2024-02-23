// Copyright 2024 Thomas Ballantyne. All rights reserved.


#include "CCGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "CCAbilitySystemComponent.h"
#include "CCTargetType.h"
#include "../CharacterBase.h"
#include "GameplayTagContainer.h"
#include "CasinoCrasher/Player/PlayerCharacter.h"

UCCGameplayAbility::UCCGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bActivateAbilityOnGranted = false;
	bActivateOnInput = true;
	bSourceObjectMustEqualCurrentWeaponToActivate = false;
	bCannotActivateWhileInteracting = true;

	// UGSAbilitySystemGlobals hasn't initialized tags yet to set ActivationBlockedTags
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.KnockedDown"));

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.BlocksInteraction"));

	InteractingTag = FGameplayTag::RequestGameplayTag("State.Interacting");
	InteractingRemovalTag = FGameplayTag::RequestGameplayTag("State.InteractingRemoval");
}

void UCCGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

FGameplayAbilityTargetDataHandle UCCGameplayAbility::MakeGameplayAbilityTargetDataHandleFromActorArray(const TArray<AActor*> TargetActors)
{
	if (TargetActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Append(TargetActors);
		return FGameplayAbilityTargetDataHandle(NewData);
	}

	return FGameplayAbilityTargetDataHandle();
}

FGameplayAbilityTargetDataHandle UCCGameplayAbility::MakeGameplayAbilityTargetDataHandleFromHitResults(const TArray<FHitResult> HitResults)
{
	FGameplayAbilityTargetDataHandle TargetData;

	for (const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(NewData);
	}

	return TargetData;
}

FCCGameplayEffectContainerSpec UCCGameplayAbility::MakeEffectContainerSpecFromContainer(const FCCGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FCCGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	ACharacterBase* AvatarCharacter = Cast<ACharacterBase>(AvatarActor);
	UCCAbilitySystemComponent* OwningASC = UCCAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			TArray<FGameplayAbilityTargetDataHandle> TargetData;
			const UCCTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			TargetTypeCDO->GetTargets(AvatarCharacter, AvatarActor, EventData, TargetData, HitResults, TargetActors);
			ReturnSpec.AddTargets(TargetData, HitResults, TargetActors);
		}

		// If we don't have an override level, use the ability level
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			//OverrideGameplayLevel = OwningASC->GetDefaultAbilityLevel();
			OverrideGameplayLevel = GetAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

FCCGameplayEffectContainerSpec UCCGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FCCGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FCCGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UCCGameplayAbility::ApplyEffectContainerSpec(const FCCGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

UObject* UCCGameplayAbility::K2_GetSourceObject(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const
{
	return GetSourceObject(Handle, &ActorInfo);
}

bool UCCGameplayAbility::BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately)
{
	UCCAbilitySystemComponent* GSASC = Cast<UCCAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (GSASC)
	{
		return GSASC->BatchRPCTryActivateAbility(InAbilityHandle, EndAbilityImmediately);
	}

	return false;
}

void UCCGameplayAbility::ExternalEndAbility()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FString UCCGameplayAbility::GetCurrentPredictionKeyStatus()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	return ASC->ScopedPredictionKey.ToString() + " is valid for more prediction: " + (ASC->ScopedPredictionKey.IsValidForMorePrediction() ? TEXT("true") : TEXT("false"));
}

bool UCCGameplayAbility::IsPredictionKeyValidForMorePrediction() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	return ASC->ScopedPredictionKey.IsValidForMorePrediction();
}

bool UCCGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (bCannotActivateWhileInteracting)
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		if (ASC->GetTagCount(InteractingTag) > ASC->GetTagCount(InteractingRemovalTag))
		{
			return false;
		}
	}

	if (bSourceObjectMustEqualCurrentWeaponToActivate)
	{
		APlayerCharacter* Hero = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
		/*if (Hero && Hero->GetCurrentWeapon() && (UObject*)Hero->GetCurrentWeapon() == GetSourceObject(Handle, ActorInfo))
		{
			return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
		}
		else
		{*/
			return false;
		//}
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

bool UCCGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) && GSCheckCost(Handle, *ActorInfo);
}

bool UCCGameplayAbility::GSCheckCost_Implementation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const
{
	return true;
}

void UCCGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	GSApplyCost(Handle, *ActorInfo, ActivationInfo);
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

/*void UCCGameplayAbility::SetHUDReticle(TSubclassOf<UCCHUDReticle> ReticleClass)
{
	AGSPlayerController* PC = Cast<AGSPlayerController>(CurrentActorInfo->PlayerController);
	if (PC)
	{
		PC->SetHUDReticle(ReticleClass);
	}
}*/

/*void UCCGameplayAbility::ResetHUDReticle()
{
	AGSPlayerController* PC = Cast<AGSPlayerController>(CurrentActorInfo->PlayerController);
	if (PC)
	{
		AGSHeroCharacter* Hero = Cast<AGSHeroCharacter>(CurrentActorInfo->AvatarActor);
		if (Hero && Hero->GetCurrentWeapon())
		{
			PC->SetHUDReticle(Hero->GetCurrentWeapon()->GetPrimaryHUDReticleClass());
		}
		else
		{
			PC->SetHUDReticle(nullptr);
		}
	}
}*/

void UCCGameplayAbility::SendTargetDataToServer(const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (IsPredictingClient())
	{
		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
		check(ASC);

		FScopedPredictionWindow	ScopedPrediction(ASC, IsPredictingClient());

		FGameplayTag ApplicationTag; // Fixme: where would this be useful?
		CurrentActorInfo->AbilitySystemComponent->CallServerSetReplicatedTargetData(CurrentSpecHandle,
			CurrentActivationInfo.GetActivationPredictionKey(), TargetData, ApplicationTag, ASC->ScopedPredictionKey);
	}
}

bool UCCGameplayAbility::IsInputPressed() const
{
	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	return Spec && Spec->InputPressed;
}

UAnimMontage* UCCGameplayAbility::GetCurrentMontageForMesh(USkeletalMeshComponent* InMesh)
{
	FAbilityMeshMontage AbilityMeshMontage;
	if (FindAbillityMeshMontage(InMesh, AbilityMeshMontage))
	{
		return AbilityMeshMontage.Montage;
	}

	return nullptr;
}

void UCCGameplayAbility::SetCurrentMontageForMesh(USkeletalMeshComponent* InMesh, UAnimMontage* InCurrentMontage)
{
	ensure(IsInstantiated());

	FAbilityMeshMontage AbilityMeshMontage;
	if (FindAbillityMeshMontage(InMesh, AbilityMeshMontage))
	{
		AbilityMeshMontage.Montage = InCurrentMontage;
	}
	else
	{
		CurrentAbilityMeshMontages.Add(FAbilityMeshMontage(InMesh, InCurrentMontage));
	}
}

bool UCCGameplayAbility::FindAbillityMeshMontage(USkeletalMeshComponent* InMesh, FAbilityMeshMontage& InAbilityMeshMontage)
{
	for (FAbilityMeshMontage& MeshMontage : CurrentAbilityMeshMontages)
	{
		if (MeshMontage.Mesh == InMesh)
		{
			InAbilityMeshMontage = MeshMontage;
			return true;
		}
	}

	return false;
}

void UCCGameplayAbility::MontageJumpToSectionForMesh(USkeletalMeshComponent* InMesh, FName SectionName)
{
	check(CurrentActorInfo);

	UCCAbilitySystemComponent* const AbilitySystemComponent = Cast<UCCAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked());
	if (AbilitySystemComponent->IsAnimatingAbilityForAnyMesh(this))
	{
		AbilitySystemComponent->CurrentMontageJumpToSectionForMesh(InMesh, SectionName);
	}
}

void UCCGameplayAbility::MontageSetNextSectionNameForMesh(USkeletalMeshComponent* InMesh, FName FromSectionName, FName ToSectionName)
{
	check(CurrentActorInfo);

	UCCAbilitySystemComponent* const AbilitySystemComponent = Cast<UCCAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked());
	if (AbilitySystemComponent->IsAnimatingAbilityForAnyMesh(this))
	{
		AbilitySystemComponent->CurrentMontageSetNextSectionNameForMesh(InMesh, FromSectionName, ToSectionName);
	}
}

void UCCGameplayAbility::MontageStopForMesh(USkeletalMeshComponent* InMesh, float OverrideBlendOutTime)
{
	check(CurrentActorInfo);

	UCCAbilitySystemComponent* const AbilitySystemComponent = Cast<UCCAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
	if (AbilitySystemComponent != nullptr)
	{
		// We should only stop the current montage if we are the animating ability
		if (AbilitySystemComponent->IsAnimatingAbilityForAnyMesh(this))
		{
			AbilitySystemComponent->CurrentMontageStopForMesh(InMesh, OverrideBlendOutTime);
		}
	}
}

void UCCGameplayAbility::MontageStopForAllMeshes(float OverrideBlendOutTime)
{
	check(CurrentActorInfo);

	UCCAbilitySystemComponent* const AbilitySystemComponent = Cast<UCCAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
	if (AbilitySystemComponent != nullptr)
	{
		if (AbilitySystemComponent->IsAnimatingAbilityForAnyMesh(this))
		{
			AbilitySystemComponent->StopAllCurrentMontages(OverrideBlendOutTime);
		}
	}
}
