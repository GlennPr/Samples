// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/NumericLimits.h"
#include "ProjectileBase.generated.h"


USTRUCT(Blueprintable)
struct FProjectileLimits
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Lifetime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RadialRange = TNumericLimits<float>::Max();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceTraveled = TNumericLimits<float>::Max();
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnCollisionSignature, UPrimitiveComponent*, OverlappedComp, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);

UCLASS(Abstract)
class AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	FOnFinishedSignature d_OnFinished;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnCollisionSignature d_OnOverlap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void UpdateTransform(float DeltaTime);
	virtual void UpdateLimits(float DeltaTime);
	virtual void PostTick(float DeltaTime);
	
	virtual float GetRadialRange();
	virtual FVector GetRadialRangeOrigin();
	virtual bool IsDone();

	// Limits
	float DistanceTraveled;
	float Lifetime;

private:

	FTransform PrevTransform;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		float Speed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		FProjectileLimits Limits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		AActor* OwnerRef;
	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//called on actor end
	virtual void Destroyed() override;

	UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void Cleanup();
};
