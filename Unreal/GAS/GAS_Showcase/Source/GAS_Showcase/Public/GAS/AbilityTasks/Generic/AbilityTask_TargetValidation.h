// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_TargetValidation.generated.h"

/**
 * 
 */

USTRUCT(Blueprintable)
struct FAbilityTriggerSettings
{
	GENERATED_USTRUCT_BODY()

public:

	FAbilityTriggerSettings()
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		bool ContinouslyCheckInstigatorTransform = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		bool CheckForRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		bool Range2DOnly = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		float DistanceTriggerOut = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		float DistanceTriggerIn = 1200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		bool CheckForAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		bool Angle2DOnly = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true),
		meta = (ClampMin = "0.0", ClampMax = "180.0", UIMin = "0.0", UIMax = "180.0"))
		float AngleTriggerOut = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true),
		meta = (ClampMin = "0.0", ClampMax = "180.0", UIMin = "0.0", UIMax = "180.0"))
		float AngleTriggerIn = 80;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTargetStatusChanged, bool, inRange_Distance, bool, inRange_Angle);


UCLASS()
class UAbilityTask_TargetValidation : public UAbilityTask
{
    GENERATED_UCLASS_BODY()

    UPROPERTY(BlueprintAssignable)
		FTargetStatusChanged d_OnTargetStatusChanged;

    virtual void Activate() override;

    virtual void OnDestroy(bool bInOwnerFinished) override;

    UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
        static UAbilityTask_TargetValidation* CheckTarget(UGameplayAbility* OwningAbility, AActor* AbilityInsigator, AActor* AbilityTarget, FAbilityTriggerSettings TriggerSettings);

private:

	AActor* Insigator = nullptr;
    AActor* Target = nullptr;
    FTimerHandle TickTimerHandle;
	FAbilityTriggerSettings Settings;
    
	FVector InstigatorLocation = FVector::ZeroVector;
	FQuat InstigatorOrientation = FQuat::Identity;
	FVector TargetLocationAtStart = FVector::ZeroVector;
	float StartingAngle;

	bool bIsFirstTick = true;
	bool TargetIsInDistanceRange = true;
	bool TargetIsInAngleRange = true;

	float GetDistance(FVector A, FVector B);
	float GetAngle(FVector AForward, FVector A, FVector B);

    UFUNCTION()
    void OnHandleTick();
};
