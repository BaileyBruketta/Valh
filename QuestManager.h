// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestManager.generated.h"

UCLASS()
class VALH_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	int NumberOfActiveQuests;

	void LoadActiveQuests();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<int> QuestNumbers;

	UFUNCTION(BlueprintCallable, Category = "Stuff")
		FString GetQuestText(int QuestNumber);
		
	UFUNCTION(BlueprintCallable, Category = "Stuff")
		FString GetQuestName(int QuestNumber);

	UFUNCTION(BlueprintCallable, Category = "Stuff")
		int GetNextQuestIfAny(int QuestNumberToCheck);

	UFUNCTION(BlueprintCallable, Category = "Stuff")
		void CompleteQuest(int QuestNumber);

	UFUNCTION(BlueprintCallable, Category = "Stuff")
		void AddQuestByNumber(int QuestNumberToAdd);
};
