// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NpcDialogueMenu.generated.h"

UCLASS()
class VALH_API ANpcDialogueMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANpcDialogueMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString DialogueGenerationPath;

	UFUNCTION(BlueprintCallable)
		void SetDialogueGenerationPath(int newPath);

	UFUNCTION(BlueprintCallable)
		FString GetResponse(int ResponseNumber);

	UFUNCTION(BlueprintCallable)
		TArray<FString> GetPlayerOptions(int OptionsNumber);

	UFUNCTION(BlueprintCallable)
		TArray<int> GetPlayerOptionNumbers(int OptionsNumber);

};
