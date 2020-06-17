// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickup.h"
#include "ResourceSpawner.generated.h"


UCLASS()
class VALH_API AResourceSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//referring to blueprint versions of the itempickup class
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class AItemPickup> ItemNumber8;

	void GenerateItemData(int BlockNumber);
	void SpawnItemsFromData(int BlockNumber);
	TArray<FString> GetBlockCoords(int BlockNumber);
	TArray<FString> GetSeedData(int BlockNumber);
	void AppendItemToDataFile(int BlockNumber, TArray<FString> ItemInfo);
	TArray<FString> GetItemData(int BlockNumber);
};
