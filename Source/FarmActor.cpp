// Fill out your copyright notice in the Description page of Project Settings.

#include "FarmActor.h"
#include "Engine.h"

// Sets default values
AFarmActor::AFarmActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	playerCharacterReference = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	numberofplants = 1000;
	FVector x = FVector(0, 0, 0);
	DirtPileLocations.Init(x, numberofplants);
	dirtBool.Init(false, numberofplants);
	plantNumber.Init(6666, numberofplants);
	plantAge.Init(0, numberofplants);
	plantWaterLevel.Init(0.0f, numberofplants);
	plantHealth.Init(100.0f, numberofplants);
	//these are technically just dirt piles - the dirt piles will handle plant spawning (maybe ..)
	spawnedPlantObjects.Init(nullptr, numberofplants);

}

// Called when the game starts or when spawned
void AFarmActor::BeginPlay()
{
	Super::BeginPlay();
	//loadplants
	PlantUpdate();
}

void AFarmActor::AddDirt(FVector location) {
	bool added = false;
	for (int i = 0; i < numberofplants; i++) {
		if (added == false) {
			if (dirtBool[i] == false) {
				AddAndGenerateDirtAtLocation(location, i);
				added = true;
			}
		}
	}
}

void AFarmActor::AddAndGenerateDirtAtLocation(FVector location, int number) {
	dirtBool[number] = true;
	DirtPileLocations[number] = location;

	FActorSpawnParameters spawnparams; FRotator c;
	APlantObject* ne = GetWorld()->SpawnActor<APlantObject>(dirtPile, DirtPileLocations[number], c, spawnparams);
	spawnedPlantObjects[number] = ne;

}

void AFarmActor::PlantUpdate() {
	//for each plant
	playerlocation = playerCharacterReference->GetActorLocation();
	for (int i = 0; i < numberofplants; i++) {
		//if there is a plant/dirt in this slot 
		if (dirtBool[i] == true) {
			//stats 
			if (plantWaterLevel[i] > 0) { plantWaterLevel[i] -= .01f; }
			if (plantWaterLevel[i] < 5) { plantHealth[i] -= .5f; }
			
			//to do - plants die


			//rendering
			//if within range
			if (distanceCheck(DirtPileLocations[i]) == true) {
				//if not yet spawned
				if (spawnedPlantObjects[i] == NULL) {
					FActorSpawnParameters spawnparams; FRotator c;
					APlantObject* ne = GetWorld()->SpawnActor<APlantObject>(dirtPile, DirtPileLocations[i], c, spawnparams);
					spawnedPlantObjects[i] = ne;
				}
			}
			else {
				//if not in range
				//if aready spawned , despawn
				if (spawnedPlantObjects[i] != NULL) {
					spawnedPlantObjects[i]->Destroy();
				}
			}

		}

	}



	GetWorld()->GetTimerManager().SetTimer(farmUpdate, this, &AFarmActor::PlantUpdate, 5.0f, false);
}
bool AFarmActor::distanceCheck(FVector plantlocation) {
	bool returnvalue = false;

	float   distance = FVector::Dist(plantlocation, playerlocation);
	if (distance < 2000.0f) {
		returnvalue = true;
	}

	return returnvalue;
}

// Called every frame
void AFarmActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

