// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//THE INVENTORY HOLDS A COLLECTION OF ITEMIDs . THESE ARE THEN REFERENCED IN GET FUNCTION TO RETURN NECESSARY VALUES TO GET CALLERS              //
//THE INVENTORY FUNCTIONS AS A NEXUS FOR VALUES THAT ARE READ BY OTHER CLASSES. ITS LIKE A GIANT KEY-PAIR DATABSE FOR ALL THE ITEMS IN THE GAME  //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


UCLASS()
class VALH_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//an array of the item ids that are in the player's current inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	TArray<int> ItemsInInventory;

	//passes an itemdata object to the inventory object to add to the inventory array, performs analyzation of object
	void AddToInventory(int ItemID);

	//This will be used to run an item ID and return a name for an item
	//These values should probably be stored in a text file and ripped line for line 
	//but we can optimize that later when i feel like looking up documentation 
	//for file writing in c++, probably when I build the save system. YOLO
	UFUNCTION(BlueprintCallable, Category = "item stats")
	FString GetItemName(int ItemID);

	UFUNCTION(BlueprintCallable, Category = "item stats")
	int GetGunNumber(int ItemID);

	//This is used to keep track of inventory spots available
	int NumberOfItemsTotal;
};
