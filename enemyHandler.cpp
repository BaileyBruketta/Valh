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
	PrimaryActorTick.bCanEverTick = false;

	//These will be changed to be enemy type specific and feature randomization like "foliagegod," but this is sufficient for testing
	numberOfEnemies = 4000;
	health.Init(100.0f, numberOfEnemies);
	spawnedEnemyReferences.Init(nullptr, numberOfEnemies);
	enemyIDList.Init(0, numberOfEnemies);
	IdsOfEnemiesInRange.Init(0, numberOfEnemies);
	timer = 60.0f;
	revivalTimer.Init(300.0f, numberOfEnemies);
	enemyTypeN.Init(0, numberOfEnemies);
	enemyCurrentBlock.Init(0, numberOfEnemies);
	playerCharacterReference = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	CurrentPlayerBlock = 0;

	NumberOfEnemiesInRange = 0;

	FVector PlaceHolder; PlaceHolder.X = 0; PlaceHolder.Y = 0; PlaceHolder.Z = 0; spawnedEnemyLocations.Init(PlaceHolder, numberOfEnemies);

	StatStringByID.Init("", numberOfEnemies);
}

// Called when the game starts or when spawned
void AenemyHandler::BeginPlay()
{
	Super::BeginPlay();
	//SpawnEnemies();
	//SaveDeloadingBlock();
	//SaveCurrentBlocks();
	for (int i = 0; i < 6; i++) {
		CreateDataFromSeed(i);
		SpawnFromBlockData(i);
	}
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
	TArray<FString> CoordData = GetBlockCoords(BlockNumber);

		//We start by Creating/Clearing save data for the block
		FString SaveDataFileName = "/EnemyBlockData" + FString::FromInt(BlockNumber) + ".txt"; FString SaveDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SaveDataFileName;
		FString ClearingText = TEXT(""); FFileHelper::SaveStringToFile(ClearingText, *SaveDataFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());

		//We first grab the seed data for this block and parse it into useable pieces

		//Grab the path of a seed data file
		FString filename = "/EnemyBlockSeedData" + FString::FromInt(BlockNumber) + ".txt";
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
				TypeFile[LineInTypeFile] = TypeFile[LineInTypeFile].Replace(TEXT(" "),TEXT(""));
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
				//randomize scale
				float scaleMin = FCString::Atof(*EnemyRootData[7]); float scaleMax = FCString::Atof(*EnemyRootData[8]);
				FString ScaleToSave       = FString::SanitizeFloat(FMath::FRandRange(scaleMin, scaleMax));
				//run location algorithm
				int xMin = FCString::Atoi(*CoordData[1]); int xMax = FCString::Atoi(*CoordData[2]); 
				int yMin = FCString::Atoi(*CoordData[3]); int yMax = FCString::Atoi(*CoordData[4]);
				FVector EnemyLocation; EnemyLocation = GenerateSpawnPoint(xMin, xMax, yMin, yMax);
				FString FX = FString::FromInt(floor(EnemyLocation[0])); 
				FString FY = FString::FromInt(floor(EnemyLocation[1]));
				FString FZ = FString::FromInt(floor(EnemyLocation[2]));
				//Write the data into file                     ; EnemyBlockSaveData : [Name,TypeNumber,Health,FVector.X,FVector.Y,FVector.Z,Rotation.X,Rotation.Y,Rotation.Z,CurrentStateNumber,WaitTimerRemaining,Scale,GlobalIdentifier]
				FString LineToSave = NameToSave + "," + TypeToSave + "," + HealthToSave + "," + FX + "," + FY + "," + FZ + "," + "0" + "," + "0" + "," + "0" + "," + StateNumberToSave + "," + WaitTimerToSave + "," + ScaleToSave + "," + "\n";
				FFileHelper::SaveStringToFile(LineToSave, *SaveDataFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
			}
			
		}
		//NumberOfEnemies = row[whateverColumnRefersTonumber]
		//for (int i = 0; i < numberOfEnemies; i++)
		//       append to CSV "EnemySaveData" Name, enemyType, health, globalidentifier, etc
		//       globalIdentifier += 1
	
}

//This is used to return an FVector for an apporpriate enemy location
FVector AenemyHandler::GenerateSpawnPoint(int xMin, int xMax, int yMin, int yMax)
{
	FVector spawnpoint;

	bool underwater = true;
	while (underwater == true)
	{
		
		spawnpoint.Z = 300.0f;
		spawnpoint.X = FMath::FRandRange(xMin, xMax);
		spawnpoint.Y = FMath::FRandRange(yMin, yMax);

		FVector HeightTest = spawnpoint; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
		FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
		if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
		{
			spawnpoint.Z = HitResult->Location.Z; //spawnpoint.Z += 400;
		}

		//avoids generating spawnpoints that are below the "water level"
		if (spawnpoint.Z > 650) { underwater = false; }
	}

	return spawnpoint;
}

//1) Relies on the block data being in order in the reference .txt file //2)Spawns in all the recorded enemies for a single block
void AenemyHandler::SpawnFromBlockData(int BlockNumber)
{
	//Start by retreiving the block data
	FString BlockDataFileName = "/EnemyBlockData" + FString::FromInt(BlockNumber) + ".txt";
	FString BlockDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + BlockDataFileName;
	TArray<FString> LoadedData; FFileHelper::LoadANSITextFileToStrings(*BlockDataFilePath, NULL, LoadedData);

	//Get default stats to assign to enemies
	//an advantage of using this method is that, even though the inital creation of enemy data takes a bit, it is very easy to manually change default values 
	//by simply editing numbers in the .txt reference files
	FString EnemyTypeDataFileName = "/EnemyTypesSpreadsheet.txt";
	FString EnemyTypeDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + EnemyTypeDataFileName;
	TArray<FString> TypeFile; FFileHelper::LoadANSITextFileToStrings(*EnemyTypeDataFilePath, NULL, TypeFile);

	//Initiate array of array of strings with a placeholder
	//parse one line of the enemytypespreadsheet into each array element of the first layer
	//an array containing an array of numbers for each enemy default
	TArray<TArray<FString>> RamshackleDatatable; RamshackleDatatable.Init(TypeFile, TypeFile.Num());
	for (int fff = 1; fff < TypeFile.Num(); fff++){TypeFile[fff].ParseIntoArray(RamshackleDatatable[fff], TEXT(","), 1);}
	TArray<FString> EnemyRootData;
	//iterate through every line of the BlockData               xyz, 345
	/////////////////////////////////////////////////////////////////////////////The -1 is a hack, it looks like the previous lines grab an empty line from EnemyBlockData, causing an array access out of bounds
	for (int i = 0; i < LoadedData.Num()-1; i++)
	{
		TArray<FString> EnemyData; LoadedData[i].ParseIntoArray(EnemyData, TEXT(","), 1);
		FVector SpawnPoint; SpawnPoint.X = FCString::Atof(*EnemyData[3]); SpawnPoint.Y = FCString::Atof(*EnemyData[4]); SpawnPoint.Z = FCString::Atof(*EnemyData[5]);
		FRotator SpawnRot;  SpawnRot.Roll = FCString::Atof(*EnemyData[6]); SpawnRot.Pitch = FCString::Atof(*EnemyData[7]); SpawnRot.Yaw = FCString::Atof(*EnemyData[8]);
		//AenemyBaseClass* newEnemy;
		FActorSpawnParameters SpawnParams; int enemyType = FCString::Atoi(*EnemyData[1]);


			//assign enemy to holding array
			bool attributed = false;int x = 0;
			while (attributed == false)
			//{for (int JJJ = 0; JJJ < spawnedEnemyReferences.Num(); JJJ++){if (spawnedEnemyReferences[JJJ] == NULL){x = JJJ;attributed = true;}}}
			{for (int JJJ = 0; JJJ < spawnedEnemyLocations.Num(); JJJ++) { if (spawnedEnemyLocations[JJJ] == FVector(0,0,0)) { x = JJJ; attributed = true; } }}
			//set stats
			for (int def = 0; def < RamshackleDatatable.Num(); def++)
			{	//set our datareference array to be the element of the ramshackle whose [1]st element's int conversion = the enemytpe of the previously spawned entity
				if (FCString::Atoi(*RamshackleDatatable[def][1]) == enemyType) { EnemyRootData = RamshackleDatatable[def]; }
			}


			FString newStatString = "";
			newStatString += EnemyRootData[3] + "," + EnemyRootData[4] + "," + EnemyRootData[5] + "," + EnemyRootData[6] + "," + EnemyRootData[9] + "," + EnemyRootData[10] + ","
				          + EnemyRootData[11] + "," + EnemyRootData[12] + "," + EnemyRootData[13] + "," + EnemyRootData[14] + ",";

			StatStringByID[x] = newStatString;
			enemyTypeN[x] = enemyType;
			health[x] = FCString::Atof(*EnemyData[2]);
			//spawnedEnemyReferences[x] = newEnemy;
			//spawnedEnemyReferences[x]->OnSpawn(this);
			int enemyID = 715 + x;
			//spawnedEnemyReferences[x]->SetGlobalID(enemyID);
			enemyIDList[x] = enemyID;
			spawnedEnemyLocations[x] = SpawnPoint;
		//}
	}
	SeeWhichEnemiesInRange();
	UpdateEnemies();
}

void AenemyHandler::BlockManager()
{
	FVector PlayerLocation = playerCharacterReference->GetActorLocation();

	//Get bounds of current block
	TArray<FString> CoordData = GetBlockCoords(CurrentPlayerBlock);
	//if player.X > blockCoord.XMax =>changeBlock(north);
}

TArray<FString> AenemyHandler::GetBlockCoords(int BlockNumber)
{
	FString BlockCoordFileName = "/BlockCoordinatesSpreadsheet.txt"; FString BlockCoordFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + BlockCoordFileName;
	TArray<FString> CoordFile; FFileHelper::LoadANSITextFileToStrings(*BlockCoordFilePath, NULL, CoordFile);
	TArray<FString> CoordData; CoordFile[BlockNumber + 1].ParseIntoArray(CoordData, TEXT(","), 1);

	return CoordData;
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
			FString FileContent = "NAME," + FString::FromInt(enemyTypeN[i]) + "," + FString::FromInt(health[i]) + "," + "\n";
			FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
		}
	}
	

}


/////////DEPRECATED////////////////DEPRECATED//////////////DEPRECATED//////////////////DEPRECATED///////////////DEPRECATED////////////////////
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

		AenemyBaseClass* newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude0, spawnpoint, spawnRot, SpawnParams);
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
	//for (int i = 0; i < numberOfEnemies; i++) { if (spawnedEnemyReferences[i] != NULL) { spawnedEnemyReferences[i]->UpdateReceived(); } }
	for (int i = 0; i < NumberOfEnemiesInRange; i++) { if (spawnedEnemyReferences[IdsOfEnemiesInRange[i]] != NULL) { spawnedEnemyReferences[IdsOfEnemiesInRange[i]]->UpdateReceived(); } }

	//Call this function again every 1 second
	GetWorld()->GetTimerManager().SetTimer(updateTimer, this, &AenemyHandler::UpdateEnemies, .05f, false);
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
		FVector LocationToSpawn = EnemyToUpdate->GetActorLocation();int enemyTypeNumber = enemyTypeN[numberToUse];
		
		
		float timer = 20;
		
		EnemyDeathDrops(numberToUse, enemyTypeNumber, LocationToSpawn);
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
	AenemyBaseClass* newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude0, spawnpoint, spawnRot, SpawnParams);
	health[oneToRevive] = 100.0f;
	spawnedEnemyReferences[oneToRevive] = newEnemy;
	spawnedEnemyReferences[oneToRevive]->OnSpawn(this);

	int enemyID = 715 + oneToRevive;
	spawnedEnemyReferences[oneToRevive]->SetGlobalID(enemyID);
	enemyIDList[oneToRevive] = enemyID;
}

//This is actually called by the instance of enemyBaseClass itself upon death, and results in both loot and a ragdoll mesh being spawned where the enemy was killed
//More code will have to be written here to ensure item persistence across sessions (loading from save) once a general mechanism for this is established
////////////////////RESET THE ENEMYSPAWNEDREFERENCE LIST WHEN THIS RUNS TO REMOVE NULL POINTERS THAT ARE SANDWICHED BETWEEN LIVE ACTORS//////////////////////////////
//TODO: CODE IN SOMETHING INITIATE A RESPAWN TIMER///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AenemyHandler::EnemyDeathDrops(int enemyID, int enemyType, FVector enemyLocation)
{
	FVector newSpot = enemyLocation; newSpot.Z += 2.0f;
	AActor* DownedEnemy = spawnedEnemyReferences[enemyID]; FVector ScaleToSpawn = DownedEnemy->GetActorScale3D();
	FRotator PlaceholderRotation = DownedEnemy->GetActorRotation(); FActorSpawnParameters PlaceholderSpawnParams;
	AActor* FirstDrop = Dummy; AActor* SecondDrop = Dummy; AActor* Ragdoll = Dummy;
	switch (enemyType)
	{
	case 0 :SpawnMeat(enemyLocation, 1);SpawnFur(enemyLocation, 1);Ragdoll = GetWorld()->SpawnActor<AActor>(Enemy0Ragdoll, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);break;
	case 4: SpawnMeat(enemyLocation, 3);SpawnFur(enemyLocation, 3);Ragdoll = GetWorld()->SpawnActor<AActor>(Enemy4Ragdoll, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);break;
	case 5: SpawnMeat(enemyLocation, 1);SpawnFur(enemyLocation, 1);Ragdoll = GetWorld()->SpawnActor<AActor>(Enemy5Ragdoll, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);break;
	case 6: SpawnMeat(enemyLocation, 3);SpawnFur(enemyLocation, 3);Ragdoll = GetWorld()->SpawnActor<AActor>(Enemy6Ragdoll, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);break;
	case 7: SpawnMeat(enemyLocation, 2);SpawnFur(enemyLocation, 2);Ragdoll = GetWorld()->SpawnActor<AActor>(Enemy7Ragdoll, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);break;
	case 8: SpawnMeat(enemyLocation, 5);SpawnFur(enemyLocation, 5);Ragdoll = GetWorld()->SpawnActor<AActor>(Enemy8Ragdoll, enemyLocation, PlaceholderRotation, PlaceholderSpawnParams);break;
	}

	//Ragdoll->SetActorScale3D(ScaleToSpawn);

}

void AenemyHandler::SpawnMeat(FVector Location, int NumberToSpawn)
{
	int x = NumberToSpawn;
	if (NumberToSpawn > 0)
	{
		FRotator PlaceholderRotation; FActorSpawnParameters PlaceholderSpawnParams;
		AActor* Dropx = GetWorld()->SpawnActor<AActor>(LootDrop1, Location, PlaceholderRotation, PlaceholderSpawnParams);
		x -= 1; SpawnMeat(Location, x);
	}
}

void AenemyHandler::SpawnFur(FVector Location, int NumberToSpawn)
{
	int x = NumberToSpawn;
	if (NumberToSpawn > 0)
	{
		FRotator PlaceholderRotation; FActorSpawnParameters PlaceholderSpawnParams;
		AActor* Drop = GetWorld()->SpawnActor<AActor>(LootDrop0, Location, PlaceholderRotation, PlaceholderSpawnParams);
		x -= 1; SpawnFur(Location, x);
	}
}

void AenemyHandler::SeeWhichEnemiesInRange()
{
	NumberOfEnemiesInRange = 0;

	//distance checks
	for (int i = 0; i < numberOfEnemies; i++)
	{
		if (spawnedEnemyLocations[i] != FVector(0,0,0))
		{
			FVector enemyLoc = spawnedEnemyLocations[i];
		
			//This cannot run until playercharacter has been spawned, so because of this, we must make player character spawn; it cannot be hand placed while this line remains in code
			FVector PlayerLocation = playerCharacterReference->GetActorLocation();
			float   distance = FVector::Dist(enemyLoc, PlayerLocation);
			if (distance < 7500)
			{
				IdsOfEnemiesInRange[NumberOfEnemiesInRange] = i;
				NumberOfEnemiesInRange += 1;
				//spawnedEnemyReferences[i]->SetActorHiddenInGame(false);
				if (spawnedEnemyReferences[i] == NULL){
				SpawnEnemyById(i); }

			}
			else if (distance > 7500) {
				if (spawnedEnemyReferences[i] != NULL) { spawnedEnemyReferences[i]->Destroy(); }
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(updateTimer2, this, &AenemyHandler::SeeWhichEnemiesInRange, 20.0f, false);
	
}

void AenemyHandler::SpawnEnemyById(int idToSpawn)
{
	
	AenemyBaseClass* newEnemy; newEnemy = Dummy;
	FActorSpawnParameters SpawnParams; FRotator SpawnRot; FVector SpawnPoint = spawnedEnemyLocations[idToSpawn];

	int Idx = enemyTypeN[idToSpawn];

	switch (Idx)
	{
	case 0: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude0, SpawnPoint, SpawnRot, SpawnParams); break;
	case 2: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude2, SpawnPoint, SpawnRot, SpawnParams); break;
	case 4: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude4, SpawnPoint, SpawnRot, SpawnParams); break;
	case 5: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude5, SpawnPoint, SpawnRot, SpawnParams); break;
	case 6: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude6, SpawnPoint, SpawnRot, SpawnParams); break;
	case 7: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude7, SpawnPoint, SpawnRot, SpawnParams); break;
	case 8: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude8, SpawnPoint, SpawnRot, SpawnParams); break;
	case 9: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude9, SpawnPoint, SpawnRot, SpawnParams); break;
	case 10: newEnemy = GetWorld()->SpawnActor<AenemyBaseClass>(enemiesToInclude10, SpawnPoint, SpawnRot, SpawnParams); break;
	}

	spawnedEnemyReferences[idToSpawn] = newEnemy;

	TArray<FString> EnemyData; StatStringByID[idToSpawn].ParseIntoArray(EnemyData, TEXT(","), 1);
	newEnemy->SetStats(FCString::Atoi(*EnemyData[0]), FCString::Atoi(*EnemyData[1]), FCString::Atoi(*EnemyData[2]),
		FCString::Atoi(*EnemyData[3]), FCString::Atoi(*EnemyData[4]), FCString::Atoi(*EnemyData[5]),
		FCString::Atoi(*EnemyData[6]), FCString::Atoi(*EnemyData[7]), FCString::Atoi(*EnemyData[8]),
		FCString::Atoi(*EnemyData[9]));

	spawnedEnemyReferences[idToSpawn]->OnSpawn(this);
	spawnedEnemyReferences[idToSpawn]->SetGlobalID(idToSpawn + 715);
}