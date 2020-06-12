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
		itemName = "Whitewater Special";
		break;
	case 1 :
		itemName = "Rebel's Rifle";
		break;
	case 2 :
	case 3 :
		itemName = "The Nazi Fucker";
		break;
	case 7 :
		itemName = "Draco, Soldier of Kings";
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
	case 1:
		gunNumber = 1;
		break;
	case 2:
	case 3:
		gunNumber = 3;
		break;
	case 7:
		gunNumber = 7;
		break;
	}

	return gunNumber;
}

int AInventory::GetGunDamage(int ItemID)
{
	int gunDamage = 6666;
	switch (ItemID)
	{
	case 0 :
		gunDamage = 40;
		break;
	case 1 : 
		gunDamage = 20;
		break;
	case 2 :
	case 3 :
		gunDamage = 9;
		break;
	case 7 :
		gunDamage = 17;
		break;
	}

	return gunDamage;
}



