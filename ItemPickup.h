// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickup.generated.h"

UCLASS()
class VALH_API AItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "item grabbed")
		int PickUpItem();

	UFUNCTION(BlueprintCallable, Category = "item grabbed")
		int GetAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemID")
		int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemID")
		int ammoInWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		bool isConsumable;

	UFUNCTION(BlueprintCallable, Category = "item grabbed")
		bool GetConsum();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		bool isWeaponOrTool;

	UFUNCTION(BlueprintCallable, Category = "item grabbed")
		bool GetWep();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		bool isResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		bool isStackable;

	UFUNCTION(BlueprintCallable, Category = "item grabbed")
		bool GetStackable();

	UFUNCTION(BlueprintCallable, Category = "item grabbed")
		bool GetRes();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		int ContentsID;  //eg. -fresh water is 1, empty is 0

	UFUNCTION(BlueprintCallable, Category = "item grabbed")
		int GetCont();

	UFUNCTION(BlueprintCallable, Category = "Initiation")
		void InitiateDefaults(TArray<FString> Defaults);

	bool BoolConvert(FString parameter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Questing")
		int QuestNumberAddedByPickingThisUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Questing")
		int QuestNumberCompletedByPickingThisUp;

	UFUNCTION(BlueprintCallable, Category = "Questing")
		int GetNumberAdded();

	UFUNCTION(BlueprintCallable, Category = "Questing")
		int GetNumberCompleted();

};
