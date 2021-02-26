// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftCalculator.h"
#include "Inventory.h"

// Sets default values
ACraftCalculator::ACraftCalculator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACraftCalculator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACraftCalculator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//this seemed like the most sensible method to check if specific items could be crafted
//I wanted to use a csv of item ids with their respective ingredients and amounts, but some items have substitutable ingredients, or even 
//what would appear to be the same ingredient to the player being represented by separate id numbers, such as is the case with different types of sticks. 
TArray<int> ACraftCalculator::getIdsOfCraftableItems()
{
	TArray<int> itemstocheck = {21};
	TArray<int> itemstoinclude; itemstoinclude.Init(6666, 1);

	for (int i = 0; i < itemstocheck.Num(); i++) {
		if (getCanCraftItem(itemstocheck[i]) == true) {
			itemstoinclude[i] = itemstocheck[i];
		}
	}

	return itemstoinclude;

}

//this gets called once for each type of craftable item by the getIdsOfCraftableItems
bool ACraftCalculator::getCanCraftItem(int ItemId)
{
	bool retval = false;

	switch (ItemId) {

	case 21: 
		int numberOfWood = 0;

		numberOfWood += PlayerInventory->NumberOfItemsWithSpecifiedId(9);
		numberOfWood += PlayerInventory->NumberOfItemsWithSpecifiedId(10);
		numberOfWood += PlayerInventory->NumberOfItemsWithSpecifiedId(11);
		numberOfWood += PlayerInventory->NumberOfItemsWithSpecifiedId(12);

		if (numberOfWood >= 3) {
			retval = true;
		}

		break;
	}

	return retval;
}

