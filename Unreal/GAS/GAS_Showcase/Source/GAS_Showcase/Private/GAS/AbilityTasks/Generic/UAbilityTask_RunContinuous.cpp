// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/Generic/UAbilityTask_RunContinuous.h"
#include "..\..\..\..\Public\GAS\AbilityTasks\Generic\UAbilityTask_RunContinuous.h"
#include "TimerManager.h"

UUAbilityTask_RunContinuous* UUAbilityTask_RunContinuous::RunContinuous(UGameplayAbility* OwningAbility)
{
	UUAbilityTask_RunContinuous* MyObj = NewAbilityTask<UUAbilityTask_RunContinuous>(OwningAbility);
	MyObj->OnAbilityCreated();
	return MyObj;
}

void UUAbilityTask_RunContinuous::OnAbilityCreated()
{
	FTimerManager& tm = GetWorld()->GetTimerManager();
	M_TimeManagerRef = &tm;
	M_TimeManagerRef->SetTimerForNextTick(this, &UUAbilityTask_RunContinuous::OnTickEvent_Listener);
}


void UUAbilityTask_RunContinuous::BeginDestroy()
{
	Super::BeginDestroy();
	M_BTickActive = false;
}

void UUAbilityTask_RunContinuous::OnTickEvent_Listener()
{
	if (M_BTickActive)
	{
		dOnTick.Broadcast(GetWorld()->GetDeltaSeconds());
		M_TimeManagerRef->SetTimerForNextTick(this, &UUAbilityTask_RunContinuous::OnTickEvent_Listener); //not sure if setting new timer is resource intensive?, Alternatively we can set a timer with near 0 time that repeats.
	}
}

