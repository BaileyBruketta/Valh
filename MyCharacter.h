// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory.h"
#include "MyCharacter.generated.h"

UCLASS()
class VALH_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	//handles e press for interacting with game world
	void Interact();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		class AInventory* Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> InventoryDefault;

	/** Sound to play each time we pick up an item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* PickupSound;

	//Active Body Mesh ; should not include arms ; should be armless
	
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshBody;

	//Active Torso Clothing
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshTorso;

	//Arms
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* Arms;

	//Active Firearm
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* Gun;

	//Active Magazine
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshMagazine;

	//GUNS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun0MeshReference;					//gun 0 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun1MeshReference;        

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun1MagazineMeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun3MeshReference;


	//This is the base function called, from here, a seconday "Hashmap" functions will be called to grab locations for guns 
	//These secondary functions will set scale andf location/ rotation global variables that will be accessed by an ADS trigger
	UFUNCTION(BlueprintCallable, Category = "Guns")
		void ChangeGunEquipped(int gunNumber);

	UFUNCTION(BlueprintCallable, Category = "Guns")
		void SetGunVariables(int gunNumber);

	FVector RelativeGunlocation;
	FVector AdsRelativeGunLocation;
	FRotator GunRotation;
	FRotator AdsGunRotation;
	FVector GunScale;

	FVector RelativeMagazineLocation;
	FVector AdsRelativeMagazineLocation;
	FRotator MagazineRotation;
	FRotator AdsMagazineRotation;
	FVector MagazineScale;

	void FireWeaponOrTool();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		float gunBaseDamage;

	//spawns bulletsmoke at point of impact when firing
	UFUNCTION()
		void SpawnBulletImpact(FVector Loc, FRotator Rot);

	//bullet smoke 
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> BulletImpact;

	//this is what is expelled from the guns barrel upon firing as smoke
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> GunSmoke;

	//timer to be used by automatic weapon firing functions
	float firearmTimer;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guns")
		int rateOfFire;

	void SemiAutomaticFire();
	void FullyAutomaticFire();

	//spawns gunsmoke upon firing
	UFUNCTION()
		void SpawnGunSmoke();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int ammoInMagazine;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	void OnFire();
	bool isFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int currentFireType;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;
	

};
