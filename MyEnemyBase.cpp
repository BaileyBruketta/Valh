// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEnemyBase.h"
#include "Engine.h"

// Sets default values
AMyEnemyBase::AMyEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	

	//adds a mesh to this abstract actor
	TargetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pickup Mesh"));

}

// Called when the game starts or when spawned
void AMyEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	Health = initHealth;
	
}

// Called every frame
void AMyEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMyEnemyBase::DamageTarget(float Damage)
{
	Health -= Damage;
	damageEventFunc();
	if (Health <= 0)
	{
		OnDefeatedTarget();
		
	}
}
void AMyEnemyBase::OnDefeatedTarget()
{
	FVector Loc;
	FRotator Rot;
	Loc = GetActorLocation();
	Rot = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(mobBodyDead, Loc, Rot, SpawnParams);
	
	Destroy();
}


