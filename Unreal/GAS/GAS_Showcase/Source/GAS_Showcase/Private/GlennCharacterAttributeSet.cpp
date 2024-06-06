// Fill out your copyright notice in the Description page of Project Settings.


#include "GlennCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"



void UGlennCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGlennCharacterAttributeSet, Health, OldHealth);
}


void UGlennCharacterAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGlennCharacterAttributeSet, HealthRegen, OldHealthRegen);
}


void UGlennCharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGlennCharacterAttributeSet, Mana, OldMana);
}


void UGlennCharacterAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGlennCharacterAttributeSet, ManaRegen, OldManaRegen);
}


void UGlennCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGlennCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGlennCharacterAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGlennCharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGlennCharacterAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
}



void UGlennCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		UAbilitySystemComponent* abilityComp = GetOwningAbilitySystemComponent();
		const float CurrentHealthValue = Health.GetBaseValue();

		NewValue = FMath::Clamp(NewValue, 0, 100);

		if (FMath::IsNearlyEqual(CurrentHealthValue, NewValue) == false)
		{
			abilityComp->ApplyModToAttributeUnsafe(Attribute, EGameplayModOp::Override, NewValue);
		}
	}


	if (Attribute == GetManaAttribute())
	{
		UAbilitySystemComponent* abilityComp = GetOwningAbilitySystemComponent();
		const float CurrentManaValue = Mana.GetBaseValue();

		NewValue = FMath::Clamp(NewValue, 0, 100);

		if (FMath::IsNearlyEqual(CurrentManaValue, NewValue) == false)
		{
			abilityComp->ApplyModToAttributeUnsafe(Attribute, EGameplayModOp::Override, NewValue);
		}
	}
}