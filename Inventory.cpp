// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Engine.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ItemsInInventory.Init(6666, 100);
	ItemCount.Init(0, 100);
	AmmoInWeapon.Init(0, 100);
	isConsum.Init(false, 100);
	isWeapon.Init(false, 100);
	isRes.Init(false, 100);
	ContentsID.Init(0, 100);
	containerAmounts.Init(0, 100);
	NumberOfItemsTotal = 0;
}

// Called when the game starts or when spawned
void AInventory::BeginPlay(){Super::BeginPlay();}

// Called every frame
void AInventory::Tick(float DeltaTime){Super::Tick(DeltaTime);}

//Add the int for an item ID 
void AInventory::AddToInventory(int ItemID, int ammo, bool consumable, bool equippable, bool resource, int contents, bool stackable)
{	
	if (stackable == true)
	{
		bool check = false;
		for (int i = 0; ItemsInInventory.Num() - 1; i++)
		{
			if (ItemsInInventory[i] == ItemID)
			{
				//add a count to an item type already represented in inventory
				ItemCount[i] += 1;
				check = true;
			}
		}

		if (check == false)
		{
			//create new item in inventory
			ItemsInInventory[NumberOfItemsTotal] = ItemID;
			AmmoInWeapon[NumberOfItemsTotal] = ammo;
			isConsum[NumberOfItemsTotal] = consumable;
			isWeapon[NumberOfItemsTotal] = equippable;
			isRes[NumberOfItemsTotal] = resource;
			ContentsID[NumberOfItemsTotal] = contents;
			NumberOfItemsTotal += 1;
		}
	}
	if (stackable == false) 
	{
		//create new item in inventory
		ItemsInInventory[NumberOfItemsTotal] = ItemID;
		AmmoInWeapon[NumberOfItemsTotal] = ammo;
		isConsum[NumberOfItemsTotal] = consumable;
		isWeapon[NumberOfItemsTotal] = equippable;
		isRes[NumberOfItemsTotal] = resource;
		ContentsID[NumberOfItemsTotal] = contents;
		NumberOfItemsTotal += 1;
	}
}

FString AInventory::GetItemName(int ItemID)
{
	FString itemName = "";
	switch (ItemID)
	{
	case 0 : itemName = "Whitewater Special";break;
	case 1 :itemName = "Freedom Fighter";break;
	case 2: break;
	case 3 : itemName = "Old English";break;
	case 5: itemName = "Fur Cloth"; break;
	case 6: itemName = "Meat Steak"; break;
	case 7 :itemName = "Short Stack";break;
	case 8:itemName = "Plastic Container"; break;
	case 9: itemName = "Log"; break;
	case 10: itemName = "Branch"; break;
	case 11: itemName = "Branch"; break;
	case 12: itemName = "Branch"; break;
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
	case 8: gunNumber = 8; break;
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
	case 8: gunDamage = 0;
	}

	return gunDamage;
}

int AInventory::GetAmmoInWeapon(int index)
{return AmmoInWeapon[index];}

FString AInventory::GetWeaponCaliber(int itemID)
{
	FString ItemCaliber = "";
	switch (itemID)
	{
	case 0: ItemCaliber = ".45 ACP"; break;
	case 1: ItemCaliber = "7.62 Russian Short"; break;
	case 3: ItemCaliber = "9 x 19 Parabellum"; break;
	case 7: ItemCaliber = "5.45 x 39 Devil's Bullet "; break;
	case 8: ItemCaliber = "Plastic Container"; break;
	case 6666: ItemCaliber = "empty hands"; break;
	}

	return ItemCaliber;
}

void AInventory::SetContainerContents(int IndexNumber, int ContentsIdentifier){ContentsID[IndexNumber] = ContentsIdentifier;}
void AInventory::SetContainerAmount(int IndexNumber, int amount) { containerAmounts[IndexNumber] = amount; }

