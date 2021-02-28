// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory.h"
#include "QuestManager.h"
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

	void AimDownSights();
	void RelaxAim();

	bool isADS;

	UFUNCTION(BlueprintCallable, category = "meta")
		bool CheckLoaderChecker();

	UFUNCTION(BlueprintCallable, category = "meta")
		void SetLoaderChecker();

	UFUNCTION(BlueprintImplementableEvent, category = Firing)
		void HitmarkerOn();

	UFUNCTION(BlueprintImplementableEvent, category = Firing)
		void HitmarkerOff();

	FTimerHandle hitmarkertimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "meta")
	USoundBase* hitsound;


	

	//float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		class AInventory* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "building")
		class ABuildCalculator* BuildCalculator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "crafting")
		class ACraftCalculator* CraftCalculator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestManager")
		class AQuestManager* QuestManager;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> InventoryDefault;

	UPROPERTY(EditDefaultsOnly, Category = "QuestManager")
		TSubclassOf<AActor> QuestManagerDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		class AFarmActor* farmClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TSubclassOf<AActor> farmClassDefault;

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

	//Arms Actual
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* LeftArm;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* RightArm;

	//Active Firearm
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* Gun;


	//Active Magazine
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshMagazine;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class UStaticMeshComponent* MeshMag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	int SeedEquipped;

	// TOOLS, WEAPONS, EQUIPMENT

	/////////////////////////////////////////////////////////////////////////////////       "Whitewater Special" (revolver)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun0MeshReference;					//gun 0 revolver

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun0AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun0FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Gun0FireSound;

	
	/////////////////////////////////////////////////////////////////////////////////       "Freedom Fighter" (ak 103)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun1MeshReference;        

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun1MagazineMeshReference;         // rifle

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun1AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun1FireAnimation;

	//////////////////////////////////////////////////////////////////////////////////       "Old English" (sten mk5)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun3MeshReference;                 // smg

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun3AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun3FireAnimation;

	/////////////////////////////////////////////////////////////////////////////////        "Short Stack" (aks74u draco)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun7MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun7WoodenParts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun7AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun7FireAnimation;

	////////////////////////////////////////////////////////////////////////////////         "Plastic Container"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun8MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun8AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun8FireAnimation;

	//////////////////////////////////////////////////////////////////////////////           "American Metal (M4A1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun10MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun10AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun10FireAnimation;

	//////////////////////////////////////////////////////////////////////////////         Wooden Axe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun20MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun20AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun20FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun20AxeHead;

	//////////////////////////////////////////////////////////////////////////////         D Day

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun24MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun24AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun24MagazineMeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimMontage* Gun24FireAnimation;

	//////////////////////////////////////////////////////////////////////////////        Backwater

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun25MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun25AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimMontage* Gun25FireAnimation;

	//////////////////////////////////////////////////////////////////////////////        Skinwalker

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun26MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun26AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimMontage* Gun26FireAnimation;

	//////////////////////////////////////////////////////////////////////////////        NT-7Z

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun28MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun28AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimMontage* Gun28FireAnimation;

	//////////////////////////////////////////////////////////////////////////////        Shovel

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun4MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun4AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimMontage* Gun4FireAnimation;

	//////////////////////////////////////////////////////////////////////////////        seed bag

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun30MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun30AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimMontage* Gun30FireAnimation;


	//Body Parts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		class USkeletalMesh* RightArmPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
		class USkeletalMesh* LeftArmPart;



	////////// /////////// /////////// /////////// //////////// ////////// /////////
	//Gun Flash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		TSubclassOf<AActor> GunFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		TSubclassOf<AActor> smallBulletCasing;


	//This is the base function called, from here, a seconday "Hashmap" functions will be called to grab locations for guns 
	//These secondary functions will set scale andf location/ rotation global variables that will be accessed by an ADS trigger
	UFUNCTION(BlueprintCallable, Category = "Guns")
		void ChangeGunEquipped(int gunNumber);

	//CEW is repeated item identifier. 3 items may have the same CEW. "WHAT" is it
	int CurrentEquippedWeapon;

	UFUNCTION(BlueprintCallable, Category = "Guns")
		void SetGunVariables(int gunNumber);

	FVector RelativeGunlocation;
	FVector AdsRelativeGunLocation;
	FRotator GunRotation;
	FRotator AdsGunRotation;
	FVector GunScale;

	FVector RelativeLeftArmLocation;
	FRotator RelativeLeftArmRotation;
	FVector RelativeRightArmLocation;
	FRotator RelativeRightArmRotation;
	FVector ArmScale;

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

	UFUNCTION()
		void DamageTarget(int damage);

	//bullet smoke 
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> BulletImpact;

	//this is what is expelled from the guns barrel upon firing as smoke
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> GunSmoke;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> BloodEffect;

	//timer to be used by automatic weapon firing functions
	float firearmTimer;

	float meleeTimer;
	void MeleeWeaponFire();
	void DecreaseMeleeTimer();
	FTimerHandle MeleePin;
	void MeleeHandler();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guns")
		float rateOfFire;

	void SemiAutomaticFire();
	void FullyAutomaticFire();

	//let the computer know that the player has stopped pressing the fire button so that it does not continue automatic fire
	void ReleaseTrigger();

	FTimerHandle firingPin;

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
	void GatherWater();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int currentFireType;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = time)
		int Hour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = time)
		int Minutes;

	void SetHoursMinutes(int H, int M);

	void GenerateClockText();
	void GenerateWeaponText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = time)
		FString ClockText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = guns)
		FString WeaponText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float HEALTH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float STAMINA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float WATER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float HUNGER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float FATIGUE;

	void UpdateStats();

	////////////////////////////////////////////////////////////////
	int buildcheck;

	void BuildMenu();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OpenBuildMenu();

	UFUNCTION(BlueprintImplementableEvent)
		void CloseBuildMenu();

	
	///////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paused")
		bool Paused;

		int pausecheck;

		//press E, run check, "Paused" grabbed by UI widget
		void PauseCheck();
		
		UFUNCTION(BlueprintCallable, Category = Guns)
			void SetAmmoInMag(int itemID);

		
		//When we change weapons or tools, it is helpful to know which slot in the inventory it occupies
		UFUNCTION(BlueprintCallable, Category = "Guns")
		void ChangeItemIndexEquipped(int IndexNumber);
		//IIE refers to element # in inventory array system, as this number can be used to manipulate multiple arrays and corresponding values. "WHERE" is it
		int InventoryIndexEquipped;

	
		UFUNCTION(BlueprintImplementableEvent, Category = "Menu")
			void ResetMenuVisibility();

		void JumpStaminaDrain();

		void StartRunning();
		void StopRunning();
		float walkspeed;
		bool running;

	/////////////////////////////////////////////////////////////////
		//Weapon Sway//
	////////////////////////////////////////////////////////////////
		FVector LookVector;

		void SwayScript(float Rate);
		float Offset;

		void NewTurnFunction(float Rate);
		void NewLookFunction(float Rate);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSway")
			float SwaySpeed;


		void SwayScriptTwo(float Rate);

		//set rotations on this to make weapon line up right 
		UPROPERTY(EditAnywhere, Category = Mesh)
			class USkeletalMeshComponent* Intermediary;

		FVector IntermediaryLocation;
		FVector ADSIntermediaryLocation;

		////////////////////recoil
		void WeaponRecoil();

		void ScrollWheelUp();
		void ScrollWheelDown();
		void ScrollWheel(int dir);
		int IndexSearching;

		//reload
		void Reload();
		void AddBullet();

		int pitchReset;

		void TryMakeDirt();
		void TryPlantSeed();

		int PlantSeedSelected;
};
