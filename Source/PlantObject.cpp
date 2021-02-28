// Fill out your copyright notice in the Description page of Project Settings.

#include "PlantObject.h"
#include "Engine.h"

// Sets default values
APlantObject::APlantObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	numseedplanted = 0;

}

// Called when the game starts or when spawned
void APlantObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlantObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantObject::PlantSeed(int seednum) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("plant seed called")));
	//play a sound 
	numseedplanted = seednum;
}

