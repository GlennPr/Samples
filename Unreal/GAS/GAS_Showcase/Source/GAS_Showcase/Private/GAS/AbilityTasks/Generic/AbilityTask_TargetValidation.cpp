// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/Generic/AbilityTask_TargetValidation.h"

UAbilityTask_TargetValidation::UAbilityTask_TargetValidation(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UAbilityTask_TargetValidation::Activate()
{
    Super::Activate();

    // Initialize any other necessary variables or setup here

    // tick every now and then, this 'should not' need to run every frame
    float ticksPerSecond = 4.0f;
    GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UAbilityTask_TargetValidation::OnHandleTick, 1.0f / ticksPerSecond, true);

    InstigatorLocation = Insigator->GetActorLocation();
    InstigatorOrientation = Insigator->GetActorRotation().Quaternion();

    if (Target != nullptr)
    {
        TargetLocationAtStart = Target->GetActorLocation();
        StartingAngle = GetAngle(InstigatorOrientation.GetForwardVector(), InstigatorLocation, TargetLocationAtStart);
    }
}


void UAbilityTask_TargetValidation::OnDestroy(bool AbilityEnded)
{
    GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
    Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_TargetValidation::OnHandleTick()
{
    if (Target != nullptr)
    {
        if (Settings.ContinouslyCheckInstigatorTransform)
        {
            InstigatorLocation = Insigator->GetActorLocation();
            InstigatorOrientation = Insigator->GetActorRotation().Quaternion();
        }

        bool bhasTargetStatusChanged = false;

        if (Settings.CheckForRange)
        {
            float distanceDiff = GetDistance(TargetLocationAtStart, Target->GetActorLocation());

            bool checkOutOfRange = TargetIsInDistanceRange || bIsFirstTick;
            if (checkOutOfRange && distanceDiff > Settings.DistanceTriggerOut)
            {
                TargetIsInDistanceRange = false;
                bhasTargetStatusChanged = true;
            }

            bool checkInRange = !TargetIsInDistanceRange || bIsFirstTick;
            if (checkInRange && distanceDiff < Settings.DistanceTriggerIn)
            {
                TargetIsInDistanceRange = true;
                bhasTargetStatusChanged = true;
            }
        }

        if (Settings.CheckForAngle)
        {
            float degreesDiff = GetAngle(InstigatorOrientation.GetForwardVector(), InstigatorLocation, Target->GetActorLocation());
            degreesDiff -= StartingAngle;

            bool checkOutOfRange = TargetIsInAngleRange || bIsFirstTick;
            if (checkOutOfRange && degreesDiff > Settings.AngleTriggerOut)
            {
                TargetIsInAngleRange = false;
                bhasTargetStatusChanged = true;
            }

            bool checkInRange = !TargetIsInAngleRange || bIsFirstTick;
            if (checkInRange && degreesDiff < Settings.AngleTriggerIn)
            {
                TargetIsInAngleRange = true;
                bhasTargetStatusChanged = true;
            }
        }

        if (bhasTargetStatusChanged)
        {
            d_OnTargetStatusChanged.Broadcast(TargetIsInDistanceRange, TargetIsInAngleRange);
        }
    }

    bIsFirstTick = false;
}

UAbilityTask_TargetValidation* UAbilityTask_TargetValidation::CheckTarget(UGameplayAbility* OwningAbility, AActor* AbilityInsigator, AActor* AbilityTarget, FAbilityTriggerSettings TriggerSettings)
{
    UAbilityTask_TargetValidation* MyObj = NewAbilityTask<UAbilityTask_TargetValidation>(OwningAbility);
    MyObj->Insigator = AbilityInsigator;
    MyObj->Target = AbilityTarget;
    MyObj->Settings = TriggerSettings;

    return MyObj;
}


float UAbilityTask_TargetValidation::GetDistance(FVector A, FVector B)
{
    FVector delta = A - B;

    if (Settings.Range2DOnly)
    {
        delta.Z = 0;
    }

    return delta.Length();
}

float UAbilityTask_TargetValidation::GetAngle(FVector AForward, FVector A, FVector B)
{
    FVector delta = A - B;
    FVector dir = delta.GetSafeNormal();

    float degreesDiff = 0.0f;
    if (Settings.Angle2DOnly)
    {
        float cosineAngle = AForward.CosineAngle2D(dir);
        degreesDiff = FMath::RadiansToDegrees(FMath::Acos(cosineAngle));
    }
    else
    {
        float DotProduct = FVector::DotProduct(AForward, dir);
        float AngleRadians = FMath::Acos(DotProduct);
        degreesDiff = FMath::RadiansToDegrees(AngleRadians);
    }

    return degreesDiff;
}
