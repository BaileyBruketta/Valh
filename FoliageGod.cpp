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
#include "FileHelper.h"
#include "Paths.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"


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
	//SpawnFunction();
	for (int i = 0; i < 6; i++) 
	{
		//GeneratePlantData(i);
		SpawnPlantsFromData(i);
	}
	
}

// Called every frame
void AFoliageGod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FString> AFoliageGod::GetBlockCoords(int BlockNumber)
{
	FString BlockCoordFileName = "/BlockCoordinatesSpreadsheet.txt"; FString BlockCoordFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + BlockCoordFileName;
	TArray<FString> CoordFile; FFileHelper::LoadANSITextFileToStrings(*BlockCoordFilePath, NULL, CoordFile);
	TArray<FString> CoordData; CoordFile[BlockNumber + 1].ParseIntoArray(CoordData, TEXT(","), 1);

	return CoordData;
}

TArray<FString> AFoliageGod::GetSeedData(int BlockNumber)
{
	FString SeedFileName = "/FoliageBlockSeedData" + FString::FromInt(BlockNumber) + ".txt";
	FString SeedFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SeedFileName;
	TArray<FString> SeedFile; FFileHelper::LoadANSITextFileToStrings(*SeedFilePath, NULL, SeedFile);

	return SeedFile;
}

void AFoliageGod::GeneratePlantData(int Block)
{
	//Clear the file
	FString SaveDataFileName = "/FoliageBlockData" + FString::FromInt(Block) + ".txt"; FString SaveDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SaveDataFileName;
	FString ClearingText = TEXT(""); FFileHelper::SaveStringToFile(ClearingText, *SaveDataFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());

	FVector Locs;
	//Get Coordinates for block
	TArray<FString> CoordData = GetBlockCoords(Block);
	//Get integer values for coords (x y bounds)
	int xMin = FCString::Atoi(*CoordData[1]); int xMax = FCString::Atoi(*CoordData[2]);
	int yMin = FCString::Atoi(*CoordData[3]); int yMax = FCString::Atoi(*CoordData[4]);
	//Get  csv line of metadata for each plant type delivered in an array
	TArray<FString> SeedFile = GetSeedData(Block);
	//Iterate through each plant type
	for (int i = 0; i < SeedFile.Num(); i++)
	{
		//grab metadata for plant                                                      0 PlantNumber, 1 min, 2 maxModifier, 3 SizeModification
		TArray<FString> PlantData; SeedFile[i].ParseIntoArray(PlantData, TEXT(","), 1);
		TArray<int>     PlantInts; PlantInts.Init(1,4);
		for (int z = 0; z < PlantData.Num(); z++) { PlantInts[z] = FCString::Atoi(*PlantData[z]); }

		//int numberOfPlants = floor(rand() % PlantInts[1] + PlantInts[2]);
		int numberOfPlants = floor(FMath::FRandRange(PlantInts[1], PlantInts[2]));

		for (int j = 0; j < numberOfPlants; j++)
		{
			//Generate a scale
			float Scale = rand() % PlantInts[3];

			//Generate a location
			bool underwater = true;
			while (underwater == true)
			{
				Locs.X = FMath::FRandRange(xMin, xMax); Locs.Y = FMath::FRandRange(yMin, yMax);
				//test height, see if new location is below the global water level
				FVector HeightTest = Locs;        HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
				FVector EndTrace = HeightTest;  EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
				if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))    //If we can cut a straight path from sky to ground
				{
					Locs.Z = HitResult->Location.Z; //Sets height to spawn plant at 
				}
				if (Locs.Z > 471) { underwater = false; } //determines if location for new plant spawn is underwater or not
			}

			int ZRot = FMath::RandRange(0, 360);

			//Save one line per plant
			TArray<FString> PlantInfo; PlantInfo.Init("",6);
			PlantInfo[0] = PlantData[0]; PlantInfo[1] = FString::SanitizeFloat(Scale);
			PlantInfo[2] = FString::SanitizeFloat(Locs.X); PlantInfo[3] = FString::SanitizeFloat(Locs.Y); PlantInfo[4] = FString::SanitizeFloat(Locs.Z);
			PlantInfo[5] = FString::FromInt(ZRot);

			AppendPlantToDataFile(Block, PlantInfo);
		}
	}
	//number,scale,x,y,z,fz
}

void AFoliageGod::AppendPlantToDataFile(int BlockNumber, TArray<FString> PlantInfo)
{
	FString PlantDataFileName = "/FoliageBlockData" + FString::FromInt(BlockNumber) + ".txt"; 
	FString PlantDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + PlantDataFileName;
	FString LineToSave = PlantInfo[0] + "," + PlantInfo[1] + "," + PlantInfo[2] + "," + PlantInfo[3] + "," + PlantInfo[4] + "," + PlantInfo[5] + "," + "\n";
	FFileHelper::SaveStringToFile(LineToSave, *PlantDataFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

TArray<FString> AFoliageGod::GetPlantData(int BlockNumber)
{
	FString PlantDataFileName = "/FoliageBlockData" + FString::FromInt(BlockNumber) + ".txt";
	FString PlantDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + PlantDataFileName;
	TArray<FString> PlantData; FFileHelper::LoadANSITextFileToStrings(*PlantDataFilePath, NULL, PlantData);

	return PlantData;
}

void AFoliageGod::SpawnPlantsFromData(int Block)
{
	FVector Locs; FRotator Rots; FVector Scale; FActorSpawnParameters SpawnParams;
	TArray<FString> PlantData = GetPlantData(Block);
	for (int i = 0; i < PlantData.Num()-1; i++)
	{
		TArray<FString> PlantDataStr; PlantData[i].ParseIntoArray(PlantDataStr, TEXT(","), 1);
		TArray<int> PlantIntegers; PlantIntegers.Init(1, 6); for (int j = 0; j < 6; j++) { PlantIntegers[j] = FCString::Atoi(*PlantDataStr[j]); }
		Locs.X = PlantIntegers[2]; Locs.Y = PlantIntegers[3]; Locs.Z = PlantIntegers[4]; Rots.Pitch = 0.0f; Rots.Roll = 0.0f;  Rots.Yaw = PlantIntegers[5]; float s = FCString::Atof(*PlantDataStr[1]);
		Scale.X = s; Scale.Y = s; Scale.Z = s;

		AActor* SpawnedActorRef;

		switch (PlantIntegers[0])
		{
		case 0: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant0, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 1: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant1, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 2: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant2, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 3: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant3, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 4: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant4, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 5: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant5, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 6: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant6, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 7: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant7, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 8: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant8, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 9: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant9, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;

		case 10: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock0, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 11: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock1, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 12: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock2, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 13: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock3, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		case 14: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock4, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;

		case 15: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant10, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
		}

	}

}

//The sheer amount of plants spawned here is made possible by using a Cull Distance Volume object native to UE4 within the map. Failure to use such support provides no inbuilt object culling feature.
//DEPRECATED

void AFoliageGod::SpawnFunction()
{
	//needed for UE4 spawn macro
	FVector  Locs = GetActorLocation();
	FRotator Rots = GetActorRotation();
	FActorSpawnParameters SpawnParams;

	//for every type of plant object we are to spawn
	for (int j = 0; j < 15; j++)
	{
		//determine number of plants by plant type
		int plantMin          = 0;
		int plantMaxModifier  = 0;
		int plantSizeModifier = 0;

		//pull differnt variables depending on which plant number is being operated on
		switch (j)
		{
			//plants
		case 0: plantMin          = 300; plantMaxModifier  = 500;plantSizeModifier = 3;break;		//TODO: have this read a CSV file with these values
		case 1: plantMin          = 3;   plantMaxModifier  = 50;  plantSizeModifier = 9;break;		//TODO: have this open a different CSV file for each terrain piece
		case 2: plantMin          = 50; plantMaxModifier  = 300;plantSizeModifier = 3;break;		
		case 3: plantMin          = 100; plantMaxModifier  = 600; plantSizeModifier = 3;break;
		case 4: plantMin          = 50; plantMaxModifier  = 300; plantSizeModifier = 3;break;
		case 5: plantMin          = 3;   plantMaxModifier  = 50;  plantSizeModifier = 3;break;
		case 6: plantMin          = 500;plantMaxModifier  = 1000;plantSizeModifier = 3;break;
		case 7: plantMin          = 5;   plantMaxModifier  = 50;  plantSizeModifier = 3;break;
			//grasses
		case 8: plantMin          = 200;plantMaxModifier  = 600;plantSizeModifier = 3;break;  //prev 30k, 45k
		case 9: plantMin = 200; plantMaxModifier = 500; plantSizeModifier = 3; break;         //prev 35k, 50k
			//rocks
		case 10: plantMin = 10; plantMaxModifier = 50; plantSizeModifier = 2; break;
		case 11: plantMin = 15; plantMaxModifier = 50; plantSizeModifier = 2; break;
		case 12: plantMin = 15; plantMaxModifier = 30; plantSizeModifier = 2; break;
		case 13: plantMin = 15; plantMaxModifier = 30; plantSizeModifier = 2; break;
		case 14: plantMin = 15; plantMaxModifier = 30; plantSizeModifier = 2; break;
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
			case 8: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant8, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 9: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(plant9, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;

			case 10: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock0, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 11: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock1, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 12: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock2, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 13: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock3, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			case 14: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(rock4, Locs, Rots, SpawnParams); SpawnedActorRef->SetActorScale3D(Scale); break;
			}

			
			//TODO: write all the relevant data for each plant inside of a CSV, so that plant data does not have to be stored in memory twice,
			//      and so that we can load and deload plants using stored data when the character moves across the map
			
		}

	}


}





