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
		TSubclassOf<class AenemyBaseClass> enemiesToInclude;

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
	TArray<int> enemyTypeID;
	TArray<bool> withinRange;
	TArray<bool> alerted;
	TArray<float> health;
	TArray<int> enemyIDList;
	TArray<float> revivalTimer;

	void Revive(int oneToRevive);
	FVector LocationOfPlayer;
	float timer;
	int currentNumber = 0;

	void DecreaseHealth(int enemyNumber, int healthDecrease);
};
