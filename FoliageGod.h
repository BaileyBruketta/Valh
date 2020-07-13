// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "FoliageGod.generated.h"

UCLASS()
class VALH_API AFoliageGod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoliageGod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant0;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant1;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant2;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant3;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant4;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant5;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant6;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant7;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant8;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant9;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock0;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock1;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock2;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock3;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock4;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant10;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant11;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant12;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant13;
	

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnFunction();

	TArray<AActor> plants;
	TArray<int> platNumber;
	TArray<FVector> plantLocations;
	TArray<float> scaleMultiplier;
	TArray<int> scaleMax;

	TArray<FString> GetBlockCoords(int BlockNumber);
	void GeneratePlantData(int Block);
	void SpawnPlantsFromData(int Block);
	TArray<FString> GetSeedData(int BlockNumber);
	void AppendPlantToDataFile(int BlockNumber, TArray<FString> PlantInfo);
	TArray<FString> GetPlantData(int BlockNumber);

	
};
