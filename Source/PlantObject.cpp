// Fill out your copyright notice in the Description page of Project Settings.

#include "PlantObject.h"

// Sets default values
APlantObject::APlantObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

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

void APlantObject::PlantSeed() {

}

