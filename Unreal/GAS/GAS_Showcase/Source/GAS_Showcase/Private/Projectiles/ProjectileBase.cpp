// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/ProjectileBase.h"
#include "Math/RotationMatrix.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (UPrimitiveComponent* Collider = (UPrimitiveComponent*) RootComponent)
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlapBegin);
	}
}

void AProjectileBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	d_OnOverlap.Broadcast(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTransform(DeltaTime);
	UpdateLimits(DeltaTime);
	
	if (IsDone())
	{
		Cleanup();
	}
	else
	{
		PostTick(DeltaTime);
	}
}

void AProjectileBase::Destroyed()
{
	Super::Destroyed();
	d_OnFinished.Broadcast();
}


void AProjectileBase::UpdateTransform(float DeltaTime)
{
	FVector L_Direction = GetActorForwardVector();
	FVector L_NewLocation = GetActorLocation() + L_Direction * Speed * DeltaTime;

	SetActorLocation(L_NewLocation, true);
}

void AProjectileBase::UpdateLimits(float DeltaTime)
{
	// Update lifetime
	Lifetime += DeltaTime;

	// Update distance traveled;
	FVector L_Displacement = GetActorLocation() - PrevTransform.GetLocation();
	DistanceTraveled += abs(L_Displacement.Size());
}

void AProjectileBase::PostTick(float DeltaTime)
{
	PrevTransform = GetActorTransform();
}

FVector AProjectileBase::GetRadialRangeOrigin()
{
	if (OwnerRef)
	{
		return OwnerRef->GetActorLocation();
	}

	ensure(GetOwner());
	if(GetOwner())
	{
		return GetOwner()->GetActorLocation();
	}
	else return FVector();
}

float AProjectileBase::GetRadialRange()
{
	FVector L_Displacement = GetActorLocation() - GetRadialRangeOrigin();
	return abs(L_Displacement.Size());
}

bool AProjectileBase::IsDone()
{
	if(Lifetime >= Limits.Lifetime) return true;
	if(DistanceTraveled >= Limits.DistanceTraveled) return true;
	if(GetRadialRange() >= Limits.RadialRange) return true;
	return false;
}

void AProjectileBase::Cleanup()
{
	d_OnFinished.Broadcast();
}
