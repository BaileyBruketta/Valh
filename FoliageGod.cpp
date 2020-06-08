// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageGod.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Engine.h"
#include "Misc/FileHelper.h"
#include "Paths.h"


// Sets default values
AFoliageGod::AFoliageGod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

// Called when the game starts or when spawned
void AFoliageGod::BeginPlay()
{
	Super::BeginPlay();
	SpawnFunction();

	
}

// Called every frame
void AFoliageGod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoliageGod::SpawnFunction()
{
	//needed for UE4 spawn macro
	FVector  Locs = GetActorLocation();
	FRotator Rots = GetActorRotation();
	FActorSpawnParameters SpawnParams;

	//for every type of plant object we are to spawn
	for (int j = 0; j < 8; j++)
	{
		//determine number of plants by plant type
		int plantMin          = 0;
		int plantMaxModifier  = 0;
		int plantSizeModifier = 0;

		//pull differnt variables depending on which plant number is being operated on
		switch (j)
		{
		case 0: plantMin          = 800; plantMaxModifier  = 1000;plantSizeModifier = 5;break;		//TODO: have this read a CSV file with these values
		case 1: plantMin          = 3;   plantMaxModifier  = 50;  plantSizeModifier = 9;break;		//TODO: have this open a different CSV file for each terrain piece
		case 2: plantMin          = 300; plantMaxModifier  = 2000;plantSizeModifier = 3;break;		
		case 3: plantMin          = 100; plantMaxModifier  = 600; plantSizeModifier = 3;break;
		case 4: plantMin          = 100; plantMaxModifier  = 600; plantSizeModifier = 3;break;
		case 5: plantMin          = 3;   plantMaxModifier  = 50;  plantSizeModifier = 3;break;
		case 6: plantMin          = 1500;plantMaxModifier  = 4000;plantSizeModifier = 3;break;
		case 7: plantMin          = 5;   plantMaxModifier  = 50;  plantSizeModifier = 3;break;
		}

		//determine the number of plants for specific plant type (J) based on stats pulled in switch case
		int numberOfPlants = floor(rand() % plantMin+plantMaxModifier);

		//For each new plant to be created
		for (int i = 0; i < numberOfPlants; i++)
		{
			bool underwater = true;
			while (underwater == true)
			{
				//new location
				float newx = (rand() % 50000) + (rand() % 50000 * -1); Locs.X = newx;
				float newy = (rand() % 50000) + (rand() % 50000 * -1); Locs.Y = newy;

				//test height, see if new location is below the global water level
				FVector HeightTest = Locs;        HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
				FVector EndTrace   = HeightTest;  EndTrace.Z   = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
				if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))    //If we can cut a straight path from sky to ground
				{
					Locs.Z = HitResult->Location.Z; //Sets height to spawn plant at 
				}

				if (Locs.Z > 471) { underwater = false; } //determines if location for new plant spawn is underwater or not
			}

			//determine scale for plant based on preset values
			FVector Scale = Locs; float f = rand() % plantSizeModifier; Scale.X = f; Scale.Y = f; Scale.Z = f;

			//Spawn a new plant, scale it
			//Though I had briefly tried using an array of plant references, i found it unnecessary complicated due to the way unreal handles C++ and pointers. It was much faster at that point in time to
			// cont. just do it this way with individual variables for each plant.
			// Ostensibly, one should be able to simply reassign a value to a variable "plant" with the switch case, and then delegate spawning to one line with this
			// cont. "plant" variable as the first argument. However, I have not done this, and instead have 8 lines of redundant spawning code. 

			AActor* SpawnedActorRef;

			switch (j)
			{
			case 0: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant0, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 1: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant1, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 2: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant2, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 3: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant3, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 4: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant4, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 5: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant5, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 6: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant6, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 7: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant7, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			}

			
			//TODO: write all the relevant data for each plant inside of a CSV, so that plant data does not have to be stored in memory twice,
			//      and so that we can load and deload plants using stored data when the character moves across the map
			
		}

	}


}





