// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "enemyHandler.generated.h"

UCLASS()
class VALH_API AenemyHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AenemyHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void UpdateEnemies();
	void CheckInRange();
	void SpawnEnemies();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TSubclassOf<class AenemyBaseClass> enemiesToInclude0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Enemies")
		TSubclassOf<class AenemyBaseClass> enemiesToInclude4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Enemies")
		TSubclassOf<class AenemyBaseClass> enemiesToInclude5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Enemies")
		TSubclassOf<class AenemyBaseClass> enemiesToInclude6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Enemies")
		TSubclassOf<class AenemyBaseClass> enemiesToInclude7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Enemies")
		TSubclassOf<class AenemyBaseClass> enemiesToInclude8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		int numberOfEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float min_X_Bound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float max_X_Bound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float min_Y_Bound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float max_Y_Bound;

	//This is a list of enemies that have been spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<AenemyBaseClass*> spawnedEnemyReferences;

	//used to perform fucntions relating to enemy statii
	TArray<FVector> spawnedEnemyLocations;
	TArray<int> enemyTypeN;
	TArray<bool> withinRange;
	TArray<bool> alerted;
	TArray<float> health;
	TArray<int> enemyIDList;
	TArray<float> revivalTimer;
	TArray<int> enemyCurrentBlock;

	void Revive(int oneToRevive);
	FVector LocationOfPlayer;
	float timer;
	int currentNumber = 0;

	void DecreaseHealth(int enemyNumber, int healthDecrease);

	FTimerHandle updateTimer;
	

	//This handles loot and drops        
	UFUNCTION()
		void EnemyDeathDrops(int enemyID, int enemyType, FVector enemyLocation);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "death")
		TSubclassOf<AActor> Enemy0Ragdoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "death")
		TSubclassOf<AActor> Enemy4Ragdoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "death")
		TSubclassOf<AActor> Enemy5Ragdoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "death")
		TSubclassOf<AActor> Enemy6Ragdoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "death")
		TSubclassOf<AActor> Enemy7Ragdoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "death")
		TSubclassOf<AActor> Enemy8Ragdoll;


	//meat slab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "death")
		TSubclassOf<AActor> LootDrop0;

	//fur slab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "death")
		TSubclassOf<AActor> LootDrop1;

	void CreateDataFromSeed(int BlockNumber);
	void SaveDeloadingBlock();
	void SaveCurrentBlocks();
	void SpawnFromBlockData(int BlockNumber);

	FVector GenerateSpawnPoint(int xMin, int xMax, int yMin, int yMax);
	
	UPROPERTY(EditAnywhere, BlueprintreadWrite, category = "Data")
		int NumberOfBlocks;

	void BlockManager();

	ACharacter* playerCharacterReference;
	int CurrentPlayerBlock;

	TArray<FString> GetBlockCoords(int BlockNumber);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "spawn")
		AenemyBaseClass* Dummy;

	void SpawnMeat(FVector Location, int NumberToSpawn);
	void SpawnFur(FVector Location, int NumberToSpawn);

};
