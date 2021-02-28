// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantObject.h"
#include "FarmActor.generated.h"

UCLASS()
class VALH_API AFarmActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFarmActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FARM")
		TArray<FVector> DirtPileLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		TArray<int> plantNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		TArray<float> plantAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		TArray<float> plantWaterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		TArray<float> plantHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		TArray<APlantObject*> spawnedPlantObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		int numberofplants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		TArray<bool> dirtBool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Enemies")
		TSubclassOf<class APlantObject> dirtPile;

	void PlantUpdate();

	UFUNCTION(Blueprintcallable, Category = "functions")
		void AddDirt(FVector location);

	UFUNCTION(Blueprintcallable, Category = "functions")
		void AddAndGenerateDirtAtLocation(FVector location, int number);


	FTimerHandle farmUpdate;
	ACharacter* playerCharacterReference;
	bool distanceCheck(FVector plantlocation);
	FVector playerlocation;
	void SeeIfPlantHasBeenPlanted(int number);
};
