// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "FireBallMagnitudeCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SHOWCASE_API UFireBallMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	UFireBallMagnitudeCalculation();

	// Override the base magnitude calculation function
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition HealthDef;
};
