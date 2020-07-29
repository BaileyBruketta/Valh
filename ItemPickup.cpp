// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"

// Sets default values
AItemPickup::AItemPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//URealItemData* constr = NewObject<URealItemData>();
	//ThisItemsData = constr;
}

// Called when the game starts or when spawned
void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemPickup::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

int AItemPickup::PickUpItem()
{
	if (ID == NULL)
	{
		ID = 0;
	}

	return ID;
}

int AItemPickup::GetAmmo()
{
	if (ammoInWeapon == NULL)
	{
		ID = 0;
	}

	return ammoInWeapon;
}

bool AItemPickup::GetConsum()    {return isConsumable;  }
bool AItemPickup::GetWep()       {return isWeaponOrTool;}
bool AItemPickup::GetRes()       {return isResource;    }
bool AItemPickup::GetStackable() {return isStackable;   }
int AItemPickup::GetCont()       {return ContentsID;    }
int AItemPickup::GetNumberAdded() { return QuestNumberAddedByPickingThisUp; }
int AItemPickup::GetNumberCompleted() { return QuestNumberCompletedByPickingThisUp; }

void AItemPickup::InitiateDefaults(TArray<FString> Defaults)
{
	
		ammoInWeapon = FCString::Atoi(*Defaults[2]);
		isConsumable = BoolConvert(Defaults[3]);
		isWeaponOrTool = BoolConvert(Defaults[4]);
		isResource = BoolConvert(Defaults[5]);
		ContentsID = FCString::Atoi(*Defaults[6]);
}

bool AItemPickup::BoolConvert(FString parameter)
{
	if (parameter == "False") { return false; }
	else return true;
}