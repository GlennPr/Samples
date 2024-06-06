// Fill out your copyright notice in the Description page of Project Settings.


#include "GlennActor.h"

// Sets default values
AGlennActor::AGlennActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGlennActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGlennActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation =  GetActorLocation() + FVector(0, 0, 100 * DeltaTime);
	SetActorLocation(newLocation);
}



