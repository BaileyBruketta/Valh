// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNPCHandler.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Math/UnrealMathUtility.h"
#include "Engine.h"
#include "NPCBaseClass.h"
#include "PawnNPC.h"
#include "Misc/FileHelper.h"

void AMyNPCHandler::BeginPlay()
{
	Gender.Init(0, numberOfEnemies);
	spawnedEnemyReferences2.Init(nullptr, numberOfEnemies);
	SpawnFromBlockData(0);
	
}

//add npc IDs and locations to memory by pulling from text, initiate the repeating "see in range" function
void AMyNPCHandler::SpawnFromBlockData(int BlockNumber)
{
	TArray<FString> SpawnPointData = GetDefaultSpawnPoints();
	TArray<FString> Details        = GetNPCDetails();

	for (int i = 0; i < SpawnPointData.Num() - 1; i++)
	{
		TArray<FString> InitialSpawnPoints; SpawnPointData[i].ParseIntoArray(InitialSpawnPoints, TEXT(","), 1);
		for (int z = 0; z < InitialSpawnPoints.Num(); z++) { InitialSpawnPoints[z] = InitialSpawnPoints[z].Replace(TEXT(" "),TEXT("")); }
		TArray<FString> NPCDetails; Details[i].ParseIntoArray(NPCDetails, TEXT(","), 1);
		FVector SpawnPoint; SpawnPoint.Set(FCString::Atof(*InitialSpawnPoints[1]), FCString::Atof(*InitialSpawnPoints[2]), FCString::Atof(*InitialSpawnPoints[3]));
		bool attributed = false; int x = 0;
		while (attributed == false)
		{
			for (int JJJ = 0; JJJ < spawnedEnemyLocations.Num(); JJJ++) { if (spawnedEnemyLocations[JJJ] == FVector(0, 0, 0)) { x = JJJ; attributed = true; } }
		}
		int NPCID = 715 + x;
		enemyIDList[x] = NPCID;
		if (NPCDetails[3].Replace(TEXT(" "),TEXT("")) == "Female") { Gender[x] = 0; } else Gender[x] = 1;
		spawnedEnemyLocations[x] = SpawnPoint;

	}

	SeeWhichEnemiesInRange();
	//UpdateEnemies();
}

TArray<FString> AMyNPCHandler::GetDefaultSpawnPoints()
{
	//Start by retreiving the default spawn points 
	FString BlockDataFileName = "/NPCDefaultLocations.txt";
	FString BlockDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + BlockDataFileName;
	TArray<FString> LoadedData; FFileHelper::LoadANSITextFileToStrings(*BlockDataFilePath, NULL, LoadedData);
	return LoadedData;
}

TArray<FString> AMyNPCHandler::GetNPCDetails()
{
	FString DetailsDataFileName = "/NPCNameSheet.txt";
	FString DetailsDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + DetailsDataFileName;
	TArray<FString> LoadedData; FFileHelper::LoadANSITextFileToStrings(*DetailsDataFilePath, NULL, LoadedData);
	return LoadedData;
}

void AMyNPCHandler::SpawnEnemyById(int idToSpawn)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("spawning")));

	//APawnNPC* newNPC; newNPC = Dummy2;

		FActorSpawnParameters SpawnParams; FRotator SpawnRot(0.0f, 0.0f, 0.0f); FVector SpawnPoint = spawnedEnemyLocations[idToSpawn];
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APawnNPC* newNPC = GetWorld()->SpawnActor<APawnNPC>(DynamicDefaultNonPlayableCharacter, SpawnPoint, SpawnRot, SpawnParams);
	
	    FVector scl; scl.Set(0.3f, 0.3f, 0.3f); newNPC->SetActorScale3D(scl);
	spawnedEnemyReferences2[idToSpawn] = newNPC;
	//switch (Gender[idToSpawn]) { case 0: Cast<APawnNPC>(newNPC)->ChangeMesh(DefaultFemaleMesh); 
	//							         newNPC->SABP(FemaleAnimBlueprintReference); break;
	//                             case 1: Cast<APawnNPC>(newNPC)->ChangeMesh(DefaultMaleMesh); break;
	//								     newNPC->SABP(MaleAnimBlueprintReference); break;
	//}

	//spawnedEnemyReferences2[idToSpawn]->OnSpawn(Cast<AenemyHandler>(this));
	//spawnedEnemyReferences2[idToSpawn]->SetGlobalID(idToSpawn + 715);

}

void AMyNPCHandler::SeeWhichEnemiesInRange()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Checking Range")));
	NumberOfEnemiesInRange = 0;

	//distance checks
	for (int i = 0; i < numberOfEnemies; i++)
	{
		if (spawnedEnemyLocations[i] != FVector(0, 0, 0))
		{
			FVector enemyLoc = spawnedEnemyLocations[i];

			//This cannot run until playercharacter has been spawned, so because of this, we must make player character spawn; it cannot be hand placed while this line remains in code
			FVector PlayerLocation = playerCharacterReference->GetActorLocation();
			float   distance = FVector::Dist(enemyLoc, PlayerLocation);
			if (distance < 7500)
			{
				IdsOfEnemiesInRange[NumberOfEnemiesInRange] = i;
				NumberOfEnemiesInRange += 1;
				if (spawnedEnemyReferences2[i] == NULL) {
					SpawnEnemyById(i);
				}

			}
			else if (distance > 7500) {
				if (spawnedEnemyReferences2[i] != NULL) { spawnedEnemyReferences2[i]->Destroy(); }
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(updateTimer3, this, &AMyNPCHandler::SeeWhichEnemiesInRange, 20.0f, false);
}

void AMyNPCHandler::UpdateEnemies()
{

}