// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enemyHandler.h"
#include "NPCBaseClass.h"
#include "PawnNPC.h"
#include "MyNPCHandler.generated.h"

/**
 * 
 */
UCLASS()
class VALH_API AMyNPCHandler : public AenemyHandler
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	//AMyNPCHandler();
	virtual void BeginPlay() override;
	virtual void SpawnFromBlockData(int BlockNumber) override;
	virtual void SpawnEnemyById(int idToSpawn) override;
	virtual void SeeWhichEnemiesInRange() override;
	FTimerHandle updateTimer3;

public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		class USkeletalMesh* DefaultMaleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		class USkeletalMesh* DefaultFemaleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* MaleAnimBlueprintReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* FemaleAnimBlueprintReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TSubclassOf<class APawnNPC> DynamicDefaultNonPlayableCharacter;

	TArray<FString> GetDefaultSpawnPoints();
	TArray<FString> GetNPCDetails();
	TArray<int> Gender;

	//This is a list of enemies that have been spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<APawnNPC*> spawnedEnemyReferences2;

	virtual void UpdateEnemies() override;


	
};
