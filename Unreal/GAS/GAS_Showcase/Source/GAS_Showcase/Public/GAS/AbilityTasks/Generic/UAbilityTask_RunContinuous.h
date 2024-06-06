// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UAbilityTask_RunContinuous.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTickOutputDelegate, float, DeltaTime);

/**
 *  Simple utility task that continuously calls out the "on tick" delegate, for if you need to run a condition check every frame
 */
UCLASS()
class UUAbilityTask_RunContinuous : public UAbilityTask
{
	GENERATED_BODY()

	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UUAbilityTask_RunContinuous* RunContinuous(UGameplayAbility* OwningAbility);

	UFUNCTION()
	void OnAbilityCreated();

	virtual void BeginDestroy() override;

	UPROPERTY()
	bool M_BTickActive = true;

	FTimerManager* M_TimeManagerRef;

	UFUNCTION()
	void OnTickEvent_Listener();

	UPROPERTY(BlueprintAssignable)
	FTickOutputDelegate dOnTick;

};
