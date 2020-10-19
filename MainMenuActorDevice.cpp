// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuActorDevice.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "FileManagerGeneric.h"

// Sets default values
AMainMenuActorDevice::AMainMenuActorDevice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainMenuActorDevice::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainMenuActorDevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//This really needs something to make sure the player isn't using the name of an already existing savefile
//ALTERNATIVELY - the actual save directory should be indexed, and there should be a table matching save files to names, allowing for identical names
void AMainMenuActorDevice::NewGame(FString newName)
{
	//Make a directory for the new player
	FString SaveDataFileName = "/SaveGames/" + newName; FString SaveDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SaveDataFileName;
	const TCHAR* x = *SaveDataFilePath;
	FFileManagerGeneric::Get().MakeDirectory(x, false);

	//set a file stating whicbh player is currently being used, to be accessed by the next loaded map
	FString LetGameKnowActiveSave = "/ActivePlayer.txt"; FString ActiveSave = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + LetGameKnowActiveSave;
	FFileHelper::SaveStringToFile(newName, *ActiveSave, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());

	SetLoadIntoCheckerToTrue();

	
}

void AMainMenuActorDevice::SetLoadIntoCheckerToTrue()
{
	FString Location = "/ToggleMainMenuAndActiveCharacterBP.txt"; FString Pathf = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + Location;
	FString savee = "true"; 
	FFileHelper::SaveStringToFile(savee, *Pathf, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
}

