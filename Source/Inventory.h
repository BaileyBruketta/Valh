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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	TArray<int> AmmoInWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<bool> isConsum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<bool> isWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<bool> isRes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<int> ContentsID;  //eg. -fresh water is 1, empty is 0

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<int> containerAmounts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<int> ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<int> isAmmo;

	//passes an itemdata object to the inventory object to add to the inventory array, performs analyzation of object
	void AddToInventory(int ItemID, int ammo, bool consumable, bool equippable, bool resource, int contents, bool stackable, bool isam);

	//This will be used to run an item ID and return a name for an item
	//These values should probably be stored in a text file and ripped line for line 
	//but we can optimize that later when i feel like looking up documentation 
	//for file writing in c++, probably when I build the save system. YOLO
	UFUNCTION(BlueprintCallable, Category = "item stats")
	FString GetItemName(int ItemID);

	UFUNCTION(BlueprintCallable, Category = "item stats")
	int GetGunNumber(int ItemID);

	UFUNCTION(BlueprintCallable, Category = "item stats")
	int GetGunDamage(int ItemID);

	//Given an item number, assuming the item is a weapon, this will return the int value of the itemID which represents this weapons ammo type. all non weapon queries return 6666
	UFUNCTION(BlueprintCallable, Category = "item stats")
		int GetItemIDForAmmoSpecificToWeaponItemID(int ItemID);

	//This is used to keep track of inventory spots available
	int NumberOfItemsTotal;

	UFUNCTION(BlueprintCallable, Category = "item stats")
		int GetAmmoInWeapon(int index);

	UFUNCTION(BlueprintCallable, Category = "item stats")
		FString GetWeaponCaliber(int itemID);

	UFUNCTION(BlueprintCallable, Category = "item stats")
		FString GetAmmoCaliber(int index);

	//we set what type of thing is hekd by a container
	UFUNCTION()
	void SetContainerContents(int IndexNumber, int ContentsIdentifier);

	//we set how much of a thing is held by a container
	UFUNCTION()
	void SetContainerAmount(int IndexNumber, int amount);

	FString AppendAmountToString(int ItemId);

	UFUNCTION(BlueprintCallable, Category = "Item Stats")
		int GetItemCountForSpecificItemID(int ItemID);

	//Given the ItemID of a weapon, this returns ID of the ammo used by this weapon
	UFUNCTION(BlueprintCallable, Category = "Item Stats")
		int GetAmmoAvailableForGivenWeapon(int ItemID);

	UFUNCTION(BlueprintCallable, Category = "Item Stats")
		int GetMagazineMax(int ItemID);

	void SpendBullet(int ItemID);

	int NumberOfItemsWithSpecifiedId(int id);


	UFUNCTION(BlueprintCallable, Category = "functions")
		void SaveInventory();

	UFUNCTION(BlueprintCallable, Category = "item stats")
		void LoadInventory();

	FString boolConv(bool in);
	bool boolConvOut(FString in);
		
};
