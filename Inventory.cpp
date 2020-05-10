// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Engine.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemsInInventory.Init(6666, 100);
	NumberOfItemsTotal = 0;
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Add the int for an item ID 
void AInventory::AddToInventory(int ItemID)
{
	
		ItemsInInventory[NumberOfItemsTotal] = ItemID;

		NumberOfItemsTotal += 1;
	

}

FString AInventory::GetItemName(int ItemID)
{
	FString itemName = "";
	switch (ItemID)
	{
	case 0 : 
		itemName = "Steely Revolver";
		break;
	}

	return itemName;

}

int AInventory::GetGunNumber(int ItemID)
{
	int gunNumber = 6666;
	switch (ItemID)
	{
	case 0 :
		gunNumber = 0;
		break;
	}

	return gunNumber;
}



