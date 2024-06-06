// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_SpawnProjectile.generated.h"

USTRUCT(Blueprintable)
struct FProjectileOverlapResult
{
	GENERATED_USTRUCT_BODY()

public:
	FProjectileOverlapResult(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult SweepResult)
	{
		this->OverlappedComp = OverlappedComp;
		this->OtherActor = OtherActor;
		this->OtherComp = OtherComp;
		this->OtherBodyIndex = OtherBodyIndex;
		this->bFromSweep = bFromSweep;
		this->HitResult = SweepResult;
	}

	FProjectileOverlapResult()
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	UPrimitiveComponent* OverlappedComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	AActor* OtherActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	UPrimitiveComponent* OtherComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	int32 OtherBodyIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bFromSweep = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FHitResult HitResult;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpawnProjectileOutputDelegate, AProjectileBase*, SpawnedProjectile, FProjectileOverlapResult, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProjectileFinishedDelegate);

UCLASS()
class UAbilityTask_SpawnProjectile : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FSpawnProjectileOutputDelegate d_OnSpawned;

	UPROPERTY(BlueprintAssignable)
	FSpawnProjectileOutputDelegate d_OnOverlap;
	
	UPROPERTY(BlueprintAssignable)
	FSpawnProjectileOutputDelegate d_OnDestroyed;
	
	UPROPERTY(BlueprintAssignable)
	FProjectileFinishedDelegate d_OnFinished;

	
	
	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category="Ability|Tasks")
	static UAbilityTask_SpawnProjectile* SpawnProjectile(UGameplayAbility* OwningAbility, FTransform SpawnTransform, TSubclassOf<AProjectileBase> Class);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Abilities")
	bool BeginSpawningActor(UGameplayAbility* OwningAbility, FTransform SpawnTransform, TSubclassOf<AProjectileBase> Class, AProjectileBase*& SpawnedActor);
	// inbetween Begin and Finish, Unreal will handle 'ExposeOnSpawn' Pins
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Abilities")
	void FinishSpawningActor(UGameplayAbility* OwningAbility, FTransform SpawnTransform, AProjectileBase* SpawnedActor);

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	UFUNCTION()
	void OnProjectileFinished_Listener();

	UFUNCTION()
	void OnProjectileBeginOverlap_Listener(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ReleaseProjectile();

protected:
	FProjectileOverlapResult LastOverlapResult;
	AProjectileBase* SpawnedProjectile;
};
