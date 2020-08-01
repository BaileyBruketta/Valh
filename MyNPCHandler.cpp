// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNPCHandler.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Math/UnrealMathUtility.h"
#include "Engine.h"
#include "NPCBaseClass.h"
#include "Misc/FileHelper.h"

void AMyNPCHandler::BeginPlay()
{
	Gender.Init(0, numberOfEnemies);
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

	AenemyBaseClass* newNPC; newNPC = Dummy;
	FActorSpawnParameters SpawnParams; FRotator SpawnRot; FVector SpawnPoint = spawnedEnemyLocations[idToSpawn];
	GetWorld()->SpawnActor<AenemyBaseClass>(DynamicDefaultNonPlayableCharacter, SpawnPoint, SpawnRot, SpawnParams);
	spawnedEnemyReferences[idToSpawn] = newNPC;
	switch (Gender[idToSpawn]) { case 0: Cast<ANPCBaseClass>(newNPC)->ChangeMesh(DefaultFemaleMesh); break; 
	                             case 1: Cast<ANPCBaseClass>(newNPC)->ChangeMesh(DefaultMaleMesh); break;
	}

	spawnedEnemyReferences[idToSpawn]->OnSpawn(Cast<AenemyHandler>(this));
	spawnedEnemyReferences[idToSpawn]->SetGlobalID(idToSpawn + 715);

}