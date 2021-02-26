// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildCalculator.h"

// Sets default values
ABuildCalculator::ABuildCalculator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildCalculator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildCalculator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<int> ABuildCalculator::getIdsOfBuildableItems()
{
	TArray<int> itemstocheck = { 21 };
	TArray<int> itemstoinclude; itemstoinclude.Init(6666, 1);

	for (int i = 0; i < itemstocheck.Num(); i++) {
		if (getCanCraftItem(itemstocheck[i]) == true) {
			itemstoinclude[i] = itemstocheck[i];
		}
	}

	return itemstoinclude;

}

//this gets called once for each type of craftable item by the getIdsOfCraftableItems
bool ABuildCalculator::getCanCraftItem(int ItemId)
{
	bool retval = false;

	switch (ItemId) {

	//build small fire
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

