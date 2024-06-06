// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/Projectiles/AbilityTask_SpawnProjectile.h"
#include "Projectiles/ProjectileBase.h"

UAbilityTask_SpawnProjectile::UAbilityTask_SpawnProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UAbilityTask_SpawnProjectile* UAbilityTask_SpawnProjectile::SpawnProjectile(UGameplayAbility* OwningAbility, FTransform SpawnTransform, TSubclassOf<AProjectileBase> InClass)
{
	UAbilityTask_SpawnProjectile* MyObj = NewAbilityTask<UAbilityTask_SpawnProjectile>(OwningAbility);
	return MyObj;
}

// ---------------------------------------------------------------------------------------

bool UAbilityTask_SpawnProjectile::BeginSpawningActor(UGameplayAbility* OwningAbility, FTransform SpawnTransform, TSubclassOf<AProjectileBase> InClass, AProjectileBase*& SpawnedActor)
{
	if (Ability && Ability->GetCurrentActorInfo()->IsNetAuthority() && ShouldBroadcastAbilityTaskDelegates())
	{
		UWorld* const World = GEngine->GetWorldFromContextObject(OwningAbility, EGetWorldErrorMode::LogAndReturnNull);
		if (World)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SpawnedProjectile = World->SpawnActor<AProjectileBase>(InClass, SpawnTransform, SpawnInfo);

			if (SpawnedProjectile != nullptr)
			{
				SpawnedActor = SpawnedProjectile;
				return true;
			}
		}
	}

	return false;
}

void UAbilityTask_SpawnProjectile::FinishSpawningActor(UGameplayAbility* OwningAbility, FTransform SpawnTransform, AProjectileBase* SpawnedActor)
{
	if (SpawnedActor)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			SpawnedActor->d_OnFinished.AddDynamic(this, &UAbilityTask_SpawnProjectile::OnProjectileFinished_Listener);
			SpawnedActor->d_OnOverlap.AddDynamic(this, &UAbilityTask_SpawnProjectile::OnProjectileBeginOverlap_Listener);

			d_OnSpawned.Broadcast(SpawnedActor, LastOverlapResult);
		}
	}
}

void UAbilityTask_SpawnProjectile::OnProjectileFinished_Listener()
{
	d_OnFinished.Broadcast();
	EndTask();
}

void UAbilityTask_SpawnProjectile::OnProjectileBeginOverlap_Listener(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// The sweep can be from the opposite actor's perspective. So theres a wrapper here to make sure OverlappedComp, OtherActor etc gets passed along as well.
	FProjectileOverlapResult OverlapResult = FProjectileOverlapResult(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	LastOverlapResult = OverlapResult;

	d_OnOverlap.Broadcast(SpawnedProjectile, LastOverlapResult);

	ReleaseProjectile();
}

void UAbilityTask_SpawnProjectile::OnDestroy(bool bInOwnerFinished)
{
	UE_LOG(LogTemp, Log, TEXT("OnDestroy UAbilityTask_SpawnProjectile : %i"), bInOwnerFinished);

	ReleaseProjectile();

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_SpawnProjectile::ReleaseProjectile()
{
	if (SpawnedProjectile && IsValid(SpawnedProjectile))
	{
		
		SpawnedProjectile->d_OnFinished.RemoveDynamic(this, &UAbilityTask_SpawnProjectile::OnProjectileFinished_Listener);
		SpawnedProjectile->d_OnOverlap.RemoveDynamic(this, &UAbilityTask_SpawnProjectile::OnProjectileBeginOverlap_Listener);

		d_OnDestroyed.Broadcast(SpawnedProjectile, LastOverlapResult);
		
		if (SpawnedProjectile && IsValid(SpawnedProjectile))
		{
			SpawnedProjectile->Destroy();
			SpawnedProjectile = nullptr;
		}
	}
}