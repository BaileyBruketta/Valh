// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyHandler.h"
#include "EnemyBaseClass.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Misc/FileHelper.h"

// Sets default values
AenemyHandler::AenemyHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//These will be changed to be enemy type specific and feature randomization like "foliagegod," but this is sufficient for testing
	numberOfEnemies = 5;
	health.Init(100.0f, numberOfEnemies);
	spawnedEnemyReferences.Init(nullptr, numberOfEnemies);
	enemyIDList.Init(0, numberOfEnemies);
	timer = 60.0f;
	revivalTimer.Init(300.0f, numberOfEnemies);
	enemyType.Init(0, numberOfEnemies);
	enemyCurrentBlock.Init(0, numberOfEnemies);

}

// Called when the game starts or when spawned
void AenemyHandler::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemies();
	SaveDeloadingBlock();
	SaveCurrentBlocks();
	CreateDataFromSeed(0);
}

// Called every frame
void AenemyHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//This is called for new games to create metadata for all the enemies that will initially populate the game world. Data is created and then saved to individual block datasheets
//This should only run either 1) once when the game is started for the first time or 2)under yet to be determinded error cases when new data is needed
//EDIT: it is now passed an argument "blocknumber" upon call, meaning this runs every time you encounter a block for the first time.
//      cont. should reduce first time start up time and removes the problem of trying to generate heightmap appropriate locations for unloaded maps
void AenemyHandler::CreateDataFromSeed(int BlockNumber)
{
	//Test
	NumberOfBlocks = BlockNumber;

	//do once for each block that needs data created for it
	for (int i = 0; i < NumberOfBlocks; i++)
	{
		//We start by Creating/Clearing save data for the block
		FString SaveDataFileName = "/EnemyBlockData" + FString::FromInt(i) + ".txt"; FString SaveDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SaveDataFileName;
		FString ClearingText = TEXT(""); FFileHelper::SaveStringToFile(ClearingText, *SaveDataFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());

		//We first grab the seed data for this block and parse it into useable pieces

		//Grab the path of a seed data file
		FString filename = "/EnemyBlockSeedData" + FString::FromInt(i) + ".txt";
		FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + filename;
		//Initiate an array
		TArray<FString> LoadedFile;
		//Load the file into an array, each line is saved as an array element. These lines are Comma Separated Values
		FFileHelper::LoadANSITextFileToStrings(*FilePath, NULL, LoadedFile);

		//Now that we have a list of creatures to seed the block with, we create data for each type of creature

		//for each element/ line from the file   -    For each type of enemy that has data for spawning stored for this block
		for (int LineInSeedData = 0; LineInSeedData < LoadedFile.Num(); LineInSeedData++)
		{
			//Initiate the value where the parse is stored
			TArray<FString> LineData;
			//Parse                                                                      //  LineData            [ 0  , 1                   , 2                   , 3         ]
			LoadedFile[LineInSeedData].ParseIntoArray(LineData, TEXT(","), 1);          //Seed Data is set up as [Name, NumberToSpawnMinimum, NumberToSpawnMaximum, TypeNumber]
			
			//determine how many enemies to make by using numbers from the seed data
			int min = FCString::Atoi(*LineData[1]); int max = FCString::Atoi(*LineData[2]);
			int NumberToCreate = floor(rand() % min + max);
			//                                                                     0   ,1         ,2             ,3           ,4           ,5           ,6           ,7       ,8       ,9
			//Pull in default data for the type of enemy   ; EnemyTypeSpreadsheet:[Name,TypeNumber,StartingHealth,WalkingSpeed,WalkingTimer,RunningSpeed,RunningTimer,ScaleMin,ScaleMax,WaitingTimer]
			//We load a spreadsheet ito memory and use it to assign values to the enemies that we are about to create
			FString EnemyTypeDataFileName = "/EnemyTypesSpreadsheet.txt";
			FString EnemyTypeDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + EnemyTypeDataFileName;
			TArray<FString> TypeFile; FFileHelper::LoadANSITextFileToStrings(*EnemyTypeDataFilePath, NULL, TypeFile);

			//Now we have to find the data for the specific enemy. We just pulled in the sheet with data for ALL enemy types, and we need only data for one type
			TArray<FString> EnemyRootData;
			//We start by getting the type number for this enemy from the seed data we pulled in 
			int EnemyType = FCString::Atoi(*LineData[3]);
			//Now we search for this enemy in the typefile
			for (int LineInTypeFile = 1; LineInTypeFile < TypeFile.Num(); LineInTypeFile++)
			{
				TArray<FString> Temp;
				TypeFile[LineInTypeFile].ParseIntoArray(Temp, TEXT(","), 1);
				//We save the specific enemy data to an array
				if (FCString::Atoi(*Temp[1]) == EnemyType) { EnemyRootData = Temp; }
			}

			//for each enemy that is to be created, we will write a line of data
			for (int enemyOfTypeBeingCreated = 0; enemyOfTypeBeingCreated < NumberToCreate; enemyOfTypeBeingCreated++)
			{	
				FString NameToSave        = EnemyRootData[0];                           //Replace the fake vectors and rotations with something else
				FString TypeToSave        = EnemyRootData[1];                            FVector LocationToSave; FRotator RotationToSave;
				FString HealthToSave      = EnemyRootData[2];
				FString StateNumberToSave = "0";
				FString WaitTimerToSave   = EnemyRootData[9];
				float scaleMin = FCString::Atof(*EnemyRootData[7]); float scaleMax = FCString::Atof(*EnemyRootData[8]);
				FString ScaleToSave       = FString::SanitizeFloat(FMath::FRandRange(scaleMin, scaleMax));
				//Run a randomization on scale for applicable enemies'

				//Write the data into file                     ; EnemyBlockSaveData : [Name,TypeNumber,Health,FVector.X,FVector.Y,FVector.Z,Rotation.X,Rotation.Y,Rotation.Z,CurrentStateNumber,WaitTimerRemaining,Scale,GlobalIdentifier]
				FString LineToSave = NameToSave + "," + TypeToSave + "," + HealthToSave + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + "0" + "," + StateNumberToSave + "," + WaitTimerToSave + "," + ScaleToSave + "," + "\n";
				FFileHelper::SaveStringToFile(LineToSave, *SaveDataFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
			}
			
		}
		//NumberOfEnemies = row[whateverColumnRefersTonumber]
		//for (int i = 0; i < numberOfEnemies; i++)
		//       append to CSV "EnemySaveData" Name, enemyType, health, globalidentifier, etc
		//       globalIdentifier += 1
	}
}

//This is used to save currently loaded enemies to a csv
//This is only used to DE - LOAD a specific block. This is not to be used on enemies that will remain loaded
void AenemyHandler::SaveDeloadingBlock()
{
	//find the player's current block. we will use a placeholder for testing
	int BlockToDeLoad;
	//TEST
	BlockToDeLoad = 0;
	//

	//create filepath
	FString filename = "/EnemyBlockData" + FString::FromInt(BlockToDeLoad) + ".txt";
	//FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + TEXT("/MessageLog.txt");
	//Severity	Code	Description	Project	File	Line	Suppression State
	//Warning	C4996	'FPaths::GameSavedDir': FPaths::GameSavedDir() has been superseded by FPaths::ProjectSavedDir().Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.VALH	C : \Users\13094\Documents\Unreal Projects\VALH 4.24\Source\VALH\enemyHandler.cpp	76

	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + filename;


	//Before we start saving shit we need to clear the file out. We can do this because all of the enemies from that block should currently be loaded to memory
	//we could add an extra layer of protetcion by just writing first and then changing the filename to overwrite what was in the last save 
	FString ClearingText = TEXT(""); FFileHelper::SaveStringToFile(ClearingText, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());




	//Find enemies that are in the deloading block
	for (int i = 0; i < numberOfEnemies; i++)
	{
		//If an array element is of the block which we are delaoding
		if (enemyCurrentBlock[i] == BlockToDeLoad)
		{
			//FString FileContent = TEXT("The text to write. \n");
			//Finish the line below
			//FString FileContent = "NAME,"+ FString::FromInt(enemyType[i])+","+FString::FromInt(health[i])+","
		}
	}
}

//this is used as a precaution in case the game crashes. It saves the data for all loaded enemies into their blocks without deloading them. The enemies saved here are not removed from memory.
//Would also be a precaution for if the player improperly shuts down the game. Prevents data loss.
void AenemyHandler::SaveCurrentBlocks()
{
	//placeholder
	int BlockToSave; BlockToSave = 0;
	//create filepath
	FString filename = "/EnemyBlockData" + FString::FromInt(BlockToSave) + ".txt";
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + filename;
	//Before we start saving shit we need to clear the file out. We can do this because all of the enemies from that block should currently be loaded to memory
	FString ClearingText = TEXT(""); FFileHelper::SaveStringToFile(ClearingText, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
	//Iterate through all enemies in memory
	for (int i = 0; i < numberOfEnemies; i++)
	{
		if (enemyCurrentBlock[i] == BlockToSave)
		{
			FString FileContent = "NAME," + FString::FromInt(enemyType[i]) + "," + FString::FromInt(health[i]) + "," + "\n";
			FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
		}
	}
	

}



/////////////THIS NEEDS TO BE REPLACED BY A  FUNCTION TO SPAWN FROM A SAVEDATA TXT////////////////////////////////////////////////////////////
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
		health[i] = 8.0f;
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
	for (int i = 0; i < numberOfEnemies; i++) { if (spawnedEnemyReferences[i] != NULL) { spawnedEnemyReferences[i]->UpdateReceived(); } }

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
		currentNumber = numberToUse; AenemyBaseClass* EnemyToUpdate = Cast<AenemyBaseClass>(spawnedEnemyReferences[numberToUse]);

		//spawn loot and ragdoll meshes
		FVector LocationToSpawn = EnemyToUpdate->GetActorLocation();int enemyTypeNumber = enemyType[numberToUse];
		EnemyDeathDrops(numberToUse, enemyTypeNumber, LocationToSpawn);
		
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

//This is actually called by the instance of enemyBaseClass itself upon death, and results in both loot and a ragdoll mesh being spawned where the enemy was killed
//More code will have to be written here to ensure item persistence across sessions (loading from save) once a general mechanism for this is established
void AenemyHandler::EnemyDeathDrops(int enemyID, int enemyType, FVector enemyLocation)
{
	FRotator PlaceholderRotation; FActorSpawnParameters PlaceholderSpawnParams;
	switch (enemyType)
	{
	case 0 :
		AActor* FirstDrop  = GetWorld()->SpawnActor<AActor>(LootDrop0, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);
		AActor* SecondDrop = GetWorld()->SpawnActor<AActor>(LootDrop1, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);
		AActor* Ragdoll    = GetWorld()->SpawnActor<AActor>(Enemy0Ragdoll, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);
	break;
	}
}
