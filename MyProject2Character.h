// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyProject2Character.generated.h"


class UInputComponent;

UCLASS(config=Game)
class AMyProject2Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_GunADS;
 

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	AMyProject2Character();

protected:
	virtual void BeginPlay();

public:

	virtual void Tick(float DeltaTime) override;

	

	//***************************************************************************************
	/** accessor function for initial Hunger*/
	UFUNCTION(BlueprintPure, Category = "Hunger")
	float GetInitialHunger();

	/** accessor function for current Hunger*/
	UFUNCTION(BlueprintPure, Category = "Hunger")
	float GetCurrentHunger();

	/**updates the player's current Hunger
	* @param Hunger This is the positive or negative amount to change Hunger by.
	*/
	UFUNCTION(BlueprintCallable, Category = "Hunger")
	void UpdateCurrentHunger(float Hunger);

	//***************************************************************************************

	//accessor for initial health
	UFUNCTION(BlueprintPure, Category = "Health")
		float GetInitialHealth();

	//accessor for current health
	UFUNCTION(BlueprintPure, Category = "Health")
		float GetCurrentHealth();

	/*updates health
	* @param Health this is the positive or negative amount to change health by
	*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateCurrentHealth(float Health);
	//*************************************************************************************



	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AMyProject2Projectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;
	


	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	//guns functions and variables 

	//as follows is a comprehensive list of guns available in game 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
	class USkeletalMesh* Revolver;					//gun 0 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
	class USkeletalMesh* AK103;						//gun 1

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
	class USkeletalMesh* NavalRevolver;				//gun 2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
	class USkeletalMesh* PlainsPercussionRifle;     //gun 3

	 

	//spawns gunsmoke upon firing
	UFUNCTION()
		void SpawnGunSmoke();

	//spawns bulletsmoke at point of impact when firing
	UFUNCTION()
		void SpawnBulletImpact(FVector Loc, FRotator Rot);
	//bullet smoke 
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> BulletImpact;
	//this is what is expelled from the guns barrel upon firing as smoke
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> GunSmoke;
	//this can be called by blueprint, to be accesed by the inventory/hud widget. Use a quasi-database to swap weapons as player equips items;
	UFUNCTION(BlueprintCallable, Category = "Guns")
		void changeGunEquipped(int gunNumber, int weaponType, int newrateOfFire, int gunDamage, int magMax, int roundsInMag);

	//These are set by changing the weapon. each weapon will carry these, and this determines firearm behaviour. This provides better performance than asking the engine to determine...
	// (cont.) which firearm is actually in use. 

	//0 = semi-automatic, 1 = fully automatic, 2 = bolt action, 3 = burst fire, 4 = melee
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int currentFireType;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guns")
		int rateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		float gunBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int currentWeaponMagazineMaximum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int currentGunNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int ammoInMagazine;

	//These are floats for tracking player ammunition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int sevenPointSixTwoByThreeNine; //7.62 x 39mm Rounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int ninebyTwentymm; //9x20 .38 Rounds

protected:
	
	/** Fires a projectile. */
	void OnFire();

	//performs raycast to pcik up items 
	void onPickUp();

	//aims down the sight
	void OnAim();

	//looses aim 
	void OnHip();

	//sets a boolean to false upon trigger release, so that automatic weapons may stop firing;
	void StopFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);


private: 
	//The Player's initial Hunger
	UPROPERTY(EditAnywhere, Category = "Hunger")
		float initialHunger;
	//The Player's current Hunger 
	UPROPERTY(EditAnywhere, Category = "Hunger")
		float currentHunger;

	//The Player's initial health
	UPROPERTY(EditAnywhere, Category = "Health")
		float initialHealth;
	//The Player's initial health
	UPROPERTY(EditAnywhere, Category = "Health")
		float currentHealth;



	//this raycast is for firing weapons
	void DoRayCast();
	void SemiAutomaticFire();
	void FullyAutomaticFire();
	void FireSoundAndAnimation();


	
	

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	//timer to dictate how frequentky the UI updates for health etc; health restore rate is also tied to this by proxy, as well as hunger and stamin and thirst  
	float SupraTimer;

	//timer to be used by automatic weapon firing functions
	float firearmTimer;

	bool isFiring;

	//This is a reference to the HUD, which is filled via blueprint; This allows easy reloading functions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HudReference")
	TSubclassOf<class UUserWidget> PlayerHUDRefAlpha;
};

