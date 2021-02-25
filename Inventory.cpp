 // Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"
#include "MyCharacter.h"
#include "Engine.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ItemsInInventory.Init(6666, 100);
	ItemCount.Init(0, 100);
	AmmoInWeapon.Init(0, 100); //ammo currently in a single weapon
	isConsum.Init(false, 100); //can item be consumed
	isWeapon.Init(false, 100);
	isRes.Init(false, 100);
	isAmmo.Init(false, 100);
	ContentsID.Init(0, 100);   //id of contents in a container item
	containerAmounts.Init(0, 100);
	NumberOfItemsTotal = 0;

}

// Called when the game starts or when spawned
void AInventory::BeginPlay(){Super::BeginPlay();
//test, player starts out with 3 pocketed rounds of .45 ACP
//AddToInventory(11, 0, false, false, false, 3, true, true);

LoadInventory();
Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GenerateWeaponText();
}

// Called every frame
void AInventory::Tick(float DeltaTime){Super::Tick(DeltaTime);}

void AInventory::SaveInventory() {
	//if datafile is not found - ie, if we have not generated a file yet 
	FString Name_Of_File_Containing_Player_Name = "/tmp/ActivePlayer.txt";
	FString filepath_for_player_name = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + Name_Of_File_Containing_Player_Name;
	TArray<FString> playernamedata; FFileHelper::LoadANSITextFileToStrings(*filepath_for_player_name, NULL, playernamedata);
	FString playername = playernamedata[0];
	FString InventoryFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + "/SaveGames/" + playername + "/PlayerInventory.txt";


	//clear
	FString ClearingText = TEXT(""); FFileHelper::SaveStringToFile(ClearingText, *InventoryFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
	//save 
	for (int i = 0; i < NumberOfItemsTotal; i++) {
		FString LineToSave = FString::FromInt(ItemsInInventory[i]) + "," +
			                 FString::FromInt(ItemCount[i])        + "," +
			                 FString::FromInt(AmmoInWeapon[i])     + "," + 
			                 boolConv(isConsum[i])                 + "," +
			                 boolConv(isWeapon[i])                 + "," +
			                 boolConv(isRes[i])                    + "," +
			                 boolConv(isAmmo[i])                   + "," +
			                 FString::FromInt(ContentsID[i])       + "," +
			                 FString::FromInt(containerAmounts[i]);

	FFileHelper::SaveStringToFile(LineToSave, *InventoryFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}


}
void AInventory::LoadInventory() {
	// if datafile is not found - ie, if we have not generated a file yet
	FString Name_Of_File_Containing_Player_Name = "/tmp/ActivePlayer.txt";
	FString filepath_for_player_name = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + Name_Of_File_Containing_Player_Name;
	TArray<FString> playernamedata; FFileHelper::LoadANSITextFileToStrings(*filepath_for_player_name, NULL, playernamedata);
	FString playername = playernamedata[0];
	FString InventoryFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + "/SaveGames/" + playername + "/PlayerInventory.txt";

	if (FPaths::FileExists(InventoryFilePath)) {
		//load data from file
		TArray<FString> InventoryData; FFileHelper::LoadANSITextFileToStrings(*InventoryFilePath, NULL, InventoryData);

		//for each line, set our inventory 
		for (int i = 0; i < InventoryData.Num(); i++) {
			TArray<FString> InventoryDataString; InventoryData[i].ParseIntoArray(InventoryDataString, TEXT(","), 1);
			ItemsInInventory[i] = FCString::Atoi(*InventoryDataString[0]);
			ItemCount[i]        = FCString::Atoi(*InventoryDataString[1]);
			AmmoInWeapon[i]     = FCString::Atoi(*InventoryDataString[2]);
			isConsum[i]         = boolConvOut(InventoryDataString[3]);
			isWeapon[i]         = boolConvOut(InventoryDataString[4]);
			isRes[i]            = boolConvOut(InventoryDataString[5]);
			isAmmo[i]           = boolConvOut(InventoryDataString[6]);
			ContentsID[i]       = FCString::Atoi(*InventoryDataString[7]);
			containerAmounts[i] = FCString::Atoi(*InventoryDataString[8]);
		}


	}
	else {
		//test, player starts out with 3 pocketed rounds of .45 ACP
		AddToInventory(11, 0, false, false, false, 3, true, true);
	}
}

bool AInventory::boolConvOut(FString in) {
	bool x = false;
	if (in == "true") {
		x = true;
	}
	return x;
}

FString AInventory::boolConv(bool in) {
	FString ret = "";
	if (in == true) {
		ret = "true";
	}
	else {
		ret = "false";
	}
	return ret;
}

//Add the int for an item ID 
void AInventory::AddToInventory(int ItemID, int ammo, bool consumable, bool equippable, bool resource, int contents, bool stackable, bool isam)
{	
	//if it is not ammo 
	if (isam == false) {
		if (stackable == true)
		{
			bool check = false;
			for (int i = 0; i < ItemsInInventory.Num() - 1; i++)
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
				ItemCount[NumberOfItemsTotal] += 1;
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
			ItemCount[NumberOfItemsTotal] += 1;
			NumberOfItemsTotal += 1;
		}
	}
	//if it is ammo
	else if (isam == true)
	{
		bool check = false;
		for (int i = 0; i < ItemsInInventory.Num() - 1; i++)
		{
			if (ItemsInInventory[i] == ItemID)
			{
				//add a count to an item type already represented in inventory
				ItemCount[i] += ammo;
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
			ItemCount[NumberOfItemsTotal] += 1;
			NumberOfItemsTotal += 1;
		}
	}
}

FString AInventory::GetItemName(int ItemID)
{
	//Pull item names into memory from text
	FString SeedFileName = "/InventoryItemNames.txt";
	FString SeedFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SeedFileName;
	TArray<FString> SeedFile; FFileHelper::LoadANSITextFileToStrings(*SeedFilePath, NULL, SeedFile);
	TArray<FString> ItemNameInString; SeedFile[ItemID].ParseIntoArray(ItemNameInString, TEXT(","), 1);
	FString itemName = ""; itemName = ItemNameInString[1];
	itemName = itemName + "(" + AppendAmountToString(ItemID) + ")";
	return itemName;
}

FString AInventory::AppendAmountToString(int ItemID)
{
	FString itemNameToAppend = ""; int x = 0;
	for (int i = 0; i < ItemsInInventory.Num(); i++) { if (ItemsInInventory[i] == ItemID) { x = ItemCount[i]; } } itemNameToAppend += FString::FromInt(x);
	return itemNameToAppend;
}


int AInventory::GetGunNumber(int ItemID)
{
	int gunNumber = 6666;
	switch (ItemID)
	{
	case 0 : gunNumber = 0;  break;
	case 1:  gunNumber = 1;  break;
	case 2:
	case 3:  gunNumber = 3;  break;
	case 7:  gunNumber = 7;  break;
	case 8:  gunNumber = 8;  break;
	case 10: gunNumber = 10; break;
	case 20: gunNumber = 20; break;
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
	case 8: gunDamage = 0; break;
	case 10: gunDamage = 17; break;
	case 20: gunDamage = 35; break;
	}

	return gunDamage;
}

int AInventory::GetAmmoInWeapon(int index)
{return AmmoInWeapon[index];}

//Used by the menu to render the amount of ammunition the player has onto the screen, based on weapon type and its active ammo type
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
	case 10: ItemCaliber = "5.56  x 45 NATO .223"; break;
	case 6666: ItemCaliber = "empty hands"; break;
	case 20: ItemCaliber = "Wooden Axe"; break;
	}

	return ItemCaliber;
}

//used by the menu to display available ammo in inventory
FString AInventory::GetAmmoCaliber(int index)
{
	FString AmmoCal = "";
	switch (index)
	{
	case 0: AmmoCal = ".45 ACP"; break;
	case 1: AmmoCal = "7.62 Russian Short"; break;
	case 2: AmmoCal = "9 x 19 Parabellum"; break;
	case 3: AmmoCal = "5.45 x 39 Devil's Bullet "; break;
	case 4: AmmoCal = "5.56  x 45 NATO .223"; break;
	}

	return AmmoCal;
}

void AInventory::SetContainerContents(int IndexNumber, int ContentsIdentifier){ContentsID[IndexNumber] = ContentsIdentifier;}
void AInventory::SetContainerAmount(int IndexNumber, int amount) { containerAmounts[IndexNumber] = amount; }

int AInventory::GetItemCountForSpecificItemID(int ItemID)
{
	int countToRet = 0;
	for (int i = 0; i < ItemsInInventory.Num() -1; i++)
	{
		if (ItemsInInventory[i] == ItemID)
		{
			countToRet = ItemCount[i];
		}
	}

	return countToRet;
}

int AInventory::GetItemIDForAmmoSpecificToWeaponItemID(int ItemID)
{
	FString SeedFileName = "/InventoryItemNames.txt";
	FString SeedFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SeedFileName;
	TArray<FString> SeedFile; FFileHelper::LoadANSITextFileToStrings(*SeedFilePath, NULL, SeedFile);
	TArray<FString> ItemNameInString; SeedFile[ItemID].ParseIntoArray(ItemNameInString, TEXT(","), 1);
	int WeaponAmmoType = FCString::Atoi(*ItemNameInString[2]);
	return WeaponAmmoType;
}

int AInventory::GetMagazineMax(int ItemID)
{
	FString SeedFileName = "/InventoryItemNames.txt";
	FString SeedFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SeedFileName;
	TArray<FString> SeedFile; FFileHelper::LoadANSITextFileToStrings(*SeedFilePath, NULL, SeedFile);
	TArray<FString> ItemNameInString; SeedFile[ItemID].ParseIntoArray(ItemNameInString, TEXT(","), 1);
	int MagazineMax = FCString::Atoi(*ItemNameInString[3]);
	return MagazineMax;
}

int AInventory::GetAmmoAvailableForGivenWeapon(int ItemID)
{
	int ammoType = GetItemIDForAmmoSpecificToWeaponItemID(ItemID);
	int ammoAvailable = GetItemCountForSpecificItemID(ammoType);

	return ammoAvailable;
}

void AInventory::SpendBullet(int ItemID)
{
	for (int i = 0; i < ItemsInInventory.Num() - 1; i++)
	{
		if (ItemsInInventory[i] == ItemID)
		{
			ItemCount[i] -= 1;
		}
	}
}

int AInventory::NumberOfItemsWithSpecifiedId(int id)
{
	int val = 0;

	return val;
}

