// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.h"
#include "BuildCalculator.generated.h"

UCLASS()
class VALH_API ABuildCalculator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildCalculator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, category = "crafting")
		TArray<int> getIdsOfBuildableItems();


	bool getCanCraftItem(int ItemId);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "defaults")
		AInventory* PlayerInventory;

};
