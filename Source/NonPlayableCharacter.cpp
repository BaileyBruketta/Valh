// Fill out your copyright notice in the Description page of Project Settings.


#include "NonPlayableCharacter.h"

// Sets default values
ANonPlayableCharacter::ANonPlayableCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANonPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

