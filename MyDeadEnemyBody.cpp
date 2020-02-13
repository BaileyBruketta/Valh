// Fill out your copyright notice in the Description page of Project Settings.

#include "MyDeadEnemyBody.h"

// Sets default values
AMyDeadEnemyBody::AMyDeadEnemyBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMyDeadEnemyBody::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyDeadEnemyBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

