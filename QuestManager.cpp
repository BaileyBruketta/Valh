// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Misc/FileHelper.h"

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//Set this to 0 as default
	NumberOfActiveQuests = 0;
	//initiate an array
	QuestNumbers.Init(6666, 100);
	
}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	LoadActiveQuests();
	
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuestManager::LoadActiveQuests()
{
	//retreive the file containing the list of active player quests
	FString ActiveQuestsLogFileName = "/PlayerActiveQuests.txt"; FString ActiveQuestsLogFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + ActiveQuestsLogFileName;
	TArray<FString> ActiveQuestsFile; FFileHelper::LoadANSITextFileToStrings(*ActiveQuestsLogFilePath, NULL, ActiveQuestsFile);

	//each line of the file represents a quest
	NumberOfActiveQuests = ActiveQuestsFile.Num();

	//do once for each active quest
	for (int i = 0; i < NumberOfActiveQuests; i++)
	{
		//create an array of quest data for the quest specified 
		TArray<FString> QuestData; ActiveQuestsFile[i].ParseIntoArray(QuestData, TEXT(","), 1);
		//
		QuestNumbers[i] = FCString::Atoi(*QuestData[0]);
	}


}

FString AQuestManager::GetQuestText(int QuestNumber)
{
	FString QuestText = "";

	//We render the quest.txt file into memory, which contains text and other details for quests, ordered by number
	FString QuestPlanFileName = "/Quests.txt"; FString QuestPlanFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + QuestPlanFileName;
	TArray<FString> QuestPlanArray; FFileHelper::LoadANSITextFileToStrings(*QuestPlanFilePath, NULL, QuestPlanArray);

	TArray<FString> QuestPlan; QuestPlanArray[QuestNumber].ParseIntoArray(QuestPlan, TEXT(","), 1);

	QuestText = QuestPlan[1];


	return QuestText;
}

FString AQuestManager::GetQuestName(int QuestNumber)
{
	FString QuestName = "";

	//We render the quest.txt file into memory, which contains text and other details for quests, ordered by number
	FString QuestPlanFileName = "/Quests.txt"; FString QuestPlanFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + QuestPlanFileName;
	TArray<FString> QuestPlanArray; FFileHelper::LoadANSITextFileToStrings(*QuestPlanFilePath, NULL, QuestPlanArray);

	TArray<FString> QuestPlan; QuestPlanArray[QuestNumber].ParseIntoArray(QuestPlan, TEXT(","), 1);

	QuestName = QuestPlan[5];

	return QuestName;
}

int AQuestManager::GetNextQuestIfAny(int QuestNumberToCheck)
{
	FString NewQuestNumber = "";

	FString QuestPlanFileName = "/Quests.txt"; FString QuestPlanFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + QuestPlanFileName;
	TArray<FString> QuestPlanArray; FFileHelper::LoadANSITextFileToStrings(*QuestPlanFilePath, NULL, QuestPlanArray);

	TArray<FString> QuestPlan; QuestPlanArray[QuestNumberToCheck].ParseIntoArray(QuestPlan, TEXT(","), 1);
	
	NewQuestNumber = QuestPlan[4];

	int NewQuestNumberInt = 6666;
	    NewQuestNumberInt = FCString::Atoi(*NewQuestNumber);

	return NewQuestNumberInt;
}

void AQuestManager::CompleteQuest(int QuestNumber)
{
	//retreive the file containing the list of active player quests
	FString ActiveQuestsLogFileName = "/PlayerActiveQuests.txt"; FString ActiveQuestsLogFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + ActiveQuestsLogFileName;
	TArray<FString> ActiveQuestsFile; FFileHelper::LoadANSITextFileToStrings(*ActiveQuestsLogFilePath, NULL, ActiveQuestsFile);

	//each line of the file represents a quest
	NumberOfActiveQuests = ActiveQuestsFile.Num();

	//
	TArray<int> QuestNumberList; QuestNumberList.Init(6666, NumberOfActiveQuests);

	//do once for each active quest
	for (int i = 0; i < NumberOfActiveQuests; i++)
	{
		//create an array of quest data for the quest specified 
		TArray<FString> QuestData; ActiveQuestsFile[i].ParseIntoArray(QuestData, TEXT(","), 1);
		//
		QuestNumbers[i]    = FCString::Atoi(*QuestData[0]);	
		QuestNumberList[i] = QuestNumbers[i];
	}

	//search for index containing completed quest number and remove it from the array
	for (int z = 0; z < NumberOfActiveQuests; z++)
	{
		if (QuestNumberList[z] == QuestNumber)
		{
			QuestNumberList[z] = 6666;
		}
	}

	//Clear the file
	FString ClearingText = TEXT(""); FFileHelper::SaveStringToFile(ClearingText, *ActiveQuestsLogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());

	int placeholder = 0;
	//Append each quest number to the file
	for (int y = 0; y < NumberOfActiveQuests; y++)
	{
		if (QuestNumberList[y] != 6666)
		{
			FString LineToSave = FString::FromInt(QuestNumberList[y]);
			FFileHelper::SaveStringToFile(LineToSave, *ActiveQuestsLogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
			placeholder += 1;
		}
	}

	//Add completed quest to the Completed Quests Log
	FString CompletedQuestsLogFileName = "/PlayerCompletedQuests.txt"; FString CompletedQuestsLogFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + CompletedQuestsLogFileName;
	FString CompletedQuestNumberString = FString::FromInt(QuestNumber);
	FFileHelper::SaveStringToFile(CompletedQuestNumberString, *CompletedQuestsLogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

	//finally, change the number of active quests value as this is referenced by the menu ui and should be accurate
	NumberOfActiveQuests = placeholder;


	//add the subsequent quest for automatically added quests
	int NextQuest  = GetNextQuestIfAny(QuestNumber);
	if (NextQuest != 6666) { AddQuestByNumber(NextQuest); }

	LoadActiveQuests();

}

void AQuestManager::AddQuestByNumber(int QuestNumberToAdd)
{
	FString ActiveQuestsLogFileName = "/PlayerActiveQuests.txt"; FString ActiveQuestsLogFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + ActiveQuestsLogFileName;
	FString LineToSave = FString::FromInt(QuestNumberToAdd);
	FFileHelper::SaveStringToFile(LineToSave, *ActiveQuestsLogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

	LoadActiveQuests();
}

