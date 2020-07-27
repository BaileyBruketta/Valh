// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcDialogueMenu.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Misc/FileHelper.h"

// Sets default values
ANpcDialogueMenu::ANpcDialogueMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANpcDialogueMenu::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANpcDialogueMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Pass the # of the dialogue menu to use
void ANpcDialogueMenu::SetDialogueGenerationPath(int newPath)
{
	FString filename = "/NPCDialogueSeed" + FString::FromInt(newPath) + ".txt";
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + filename;

	DialogueGenerationPath = FilePath;

	//TArray<FString> LoadedFile; FFileHelper::LoadANSITextFileToStrings(*FilePath, NULL, LoadedFile);
}

FString ANpcDialogueMenu::GetResponse(int ResponseNumber)
{
	FString ResponseToReturn = "";
	TArray<FString> LoadedFile; FFileHelper::LoadANSITextFileToStrings(*DialogueGenerationPath, NULL, LoadedFile);
	//add response lines to array and idenfity header of R# that matches the numebr getting passed to this initial argument
	for (int i = 0; i < LoadedFile.Num() - 1; i++)
	{
		if (LoadedFile[i][0] == 'R')
		{
			//Responses[i] = LoadedFile[i];
			FString te = LoadedFile[i];
			TArray<FString> Temp; te.ParseIntoArray(Temp, TEXT(":"), 1);
			FString Le = Temp[1];
			if (Temp[0] == "R" + FString::FromInt(ResponseNumber))
			{
				ResponseToReturn = Le;
			}
			
		}
	}
	return ResponseToReturn;
}

TArray<FString> ANpcDialogueMenu::GetPlayerOptions(int OptionsNumber)
{
	TArray<FString> x; x.Init("", 6);
	TArray<FString> LoadedFile; FFileHelper::LoadANSITextFileToStrings(*DialogueGenerationPath, NULL, LoadedFile);
	//get number of option lines in text file
	for (int i = 0; i < LoadedFile.Num() - 1; i++)
	{
		if (LoadedFile[i][0] == 'P')
		{
			TArray<FString> Temp; LoadedFile[i].ParseIntoArray(Temp, TEXT(":"), 1);
			if (Temp[0] == "PDO" + FString::FromInt(OptionsNumber))
			{
				TArray<FString> Temp2; Temp[1].ParseIntoArray(Temp2, TEXT(","), 1);
				for (int z = 0; z < Temp2.Num(); z++)
				{
					x[z] = Temp2[z];
				}
			}
		}
	}
	return x;
}

TArray<int> ANpcDialogueMenu::GetPlayerOptionNumbers(int OptionsNumber)
{
	TArray<int> x; x.Init(6666, 6);
	TArray<FString> LoadedFile; FFileHelper::LoadANSITextFileToStrings(*DialogueGenerationPath, NULL, LoadedFile);

	for (int i = 0; i < LoadedFile.Num() - 1; i++)
	{
		if (LoadedFile[i][0] == 'P')
		{
			TArray<FString> Temp; LoadedFile[i].ParseIntoArray(Temp, TEXT(":"), 1);
			if (Temp[0] == "PDO" + FString::FromInt(OptionsNumber))
			{
				TArray<FString> Temp2; Temp[1].ParseIntoArray(Temp2, TEXT(","), 1);
				for (int z = 0; z < Temp2.Num(); z++)
				{
					TArray<FString> Temp3; Temp2[z].ParseIntoArray(Temp3, TEXT("$"), 1);
					x[z] = FCString::Atoi(*Temp3[0]);
				}
			}
		}
	}
	return x;

}
