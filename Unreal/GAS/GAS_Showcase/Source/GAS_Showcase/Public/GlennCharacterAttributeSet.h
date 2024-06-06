// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GlennCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GAS_SHOWCASE_API UGlennCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Health", ReplicatedUsing = OnRep_Health) // BlueprintReadOnly
	FGameplayAttributeData Health = 100.0f;
	ATTRIBUTE_ACCESSORS(UGlennCharacterAttributeSet, Health)
	
	UPROPERTY(BlueprintReadWrite, Category = "Health", ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen = 20.0f;
	ATTRIBUTE_ACCESSORS(UGlennCharacterAttributeSet, HealthRegen)


	UPROPERTY(BlueprintReadWrite, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana = 100.0f;
	ATTRIBUTE_ACCESSORS(UGlennCharacterAttributeSet, Mana)

	UPROPERTY(BlueprintReadWrite, Category = "Mana", ReplicatedUsing = OnRep_ManaRegen)
	FGameplayAttributeData ManaRegen = 30.0f;
	ATTRIBUTE_ACCESSORS(UGlennCharacterAttributeSet, ManaRegen)


	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	FGameplayAttributeData Damage = 30.0f;
	ATTRIBUTE_ACCESSORS(UGlennCharacterAttributeSet, Damage)


	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};

