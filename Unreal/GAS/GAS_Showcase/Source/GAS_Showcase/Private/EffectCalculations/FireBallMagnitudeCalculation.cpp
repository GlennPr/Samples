// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectCalculations/FireBallMagnitudeCalculation.h"
#include "GlennCharacterAttributeSet.h"


UFireBallMagnitudeCalculation::UFireBallMagnitudeCalculation()
{
    HealthDef.AttributeToCapture = UGlennCharacterAttributeSet::GetHealthAttribute();
    HealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    HealthDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(HealthDef);
}

float UFireBallMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();


    FAggregatorEvaluateParameters EvaluationParamaters;
    EvaluationParamaters.SourceTags = SourceTags;
    EvaluationParamaters.TargetTags = TargetTags;

    float health = 0.0f;
    GetCapturedAttributeMagnitude(HealthDef, Spec, EvaluationParamaters, health);

    int32 AbilityLevel = Spec.GetLevel();
    float BaseDamage = 20.0f; // get from an attribute of sorts

    float AdjustedDamage = BaseDamage * (1.0f + (AbilityLevel / 10.0f));

    return -AdjustedDamage;
}
