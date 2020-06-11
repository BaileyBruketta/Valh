// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyHandler.h"
#include "EnemyBaseClass.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

// Sets default values
AenemyHandler::AenemyHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	numberOfEnemies = 5;
	health.Init(100.0f, numberOfEnemies);
	spawnedEnemyReferences.Init(nullptr, numberOfEnemies);
	enemyIDList.Init(0, numberOfEnemies);
	timer = 60.0f;
	revivalTimer.Init(300.0f, numberOfEnemies);

}

// Called when the game starts or when spawned
void AenemyHandler::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemies();
}

// Called every frame
void AenemyHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AenemyHandler::SpawnEnemies()
{
	int x = 0;
	for (int i = 0; i < numberOfEnemies; i++)
	{

		FActorSpawnParameters SpawnParams;
		FVector spawnpoint;
		FRotator spawnRot;

		bool underwater = true;
		while (underwater == true)
		{

		
		spawnpoint.Z = 300.0f;
		spawnpoint.X = FMath::FRandRange(min_X_Bound, max_X_Bound);
		spawnpoint.Y = FMath::FRandRange(min_Y_Bound, max_Y_Bound);
		spawnRot.Yaw = 0.0f;
		spawnRot.Pitch = 0.0f;
		spawnRot.Roll = 0.0f;

		
			

			//test height
			FVector HeightTest = spawnpoint; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				spawnpoint.Z = HitResult->Location.Z; spawnpoint.Z += 400;
			}

			if (spawnpoint.Z > 471) { underwater = false; }


		}

		AenemyBaseClass* newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude, spawnpoint, spawnRot, SpawnParams);
		health[i] = 100.0f;
		spawnedEnemyReferences[i] = newEnemy;
		spawnedEnemyReferences[i]->OnSpawn(this);

		int enemyID = 715 + i;
		spawnedEnemyReferences[i]->SetGlobalID(enemyID);
		enemyIDList[i] = enemyID;

	}

	//initaite update sequence
	UpdateEnemies();
}

void AenemyHandler::UpdateEnemies()
{
	//Send update signal to all spawned enemies
	for (int i = 0; i < numberOfEnemies; i++){spawnedEnemyReferences[i]->UpdateReceived();}

	//Call this function again every 1 second
	GetWorld()->GetTimerManager().SetTimer(updateTimer, this, &AenemyHandler::UpdateEnemies, 0.05f, false);
}

void AenemyHandler::CheckInRange()
{

}

void AenemyHandler::DecreaseHealth(int enemyNumber, int healthDecrease)
{
	//enemy number is equivalent to the globalID
	int numberToUse = enemyNumber - 715;

	health[numberToUse] -= healthDecrease;

	if (health[numberToUse] <= 0)
	{
		currentNumber = numberToUse;
		AenemyBaseClass* EnemyToUpdate = Cast<AenemyBaseClass>(spawnedEnemyReferences[numberToUse]);
		EnemyToUpdate->DeathDrop();
		float timer = 20;
		spawnedEnemyReferences[numberToUse]->Destroy();
	}
}

void AenemyHandler::Revive(int oneToRevive)
{
	FActorSpawnParameters SpawnParams;
	FVector spawnpoint;
	FRotator spawnRot;
	spawnpoint.Z = 300.0f;
	spawnpoint.X = FMath::FRandRange(min_X_Bound, max_X_Bound);
	spawnpoint.Y = FMath::FRandRange(min_Y_Bound, max_Y_Bound);
	spawnRot.Yaw = 0.0f;
	spawnRot.Pitch = 0.0f;
	spawnRot.Roll = 0.0f;
	AenemyBaseClass* newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude, spawnpoint, spawnRot, SpawnParams);
	health[oneToRevive] = 100.0f;
	spawnedEnemyReferences[oneToRevive] = newEnemy;
	spawnedEnemyReferences[oneToRevive]->OnSpawn(this);

	int enemyID = 715 + oneToRevive;
	spawnedEnemyReferences[oneToRevive]->SetGlobalID(enemyID);
	enemyIDList[oneToRevive] = enemyID;
}
