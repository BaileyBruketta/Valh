// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProject2Character.h"
#include "MyProject2Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Engine.h"
#include "MyEnemyBase.h"



DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMyProject2Character

AMyProject2Character::AMyProject2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	//FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	//Iron  Sights
	FP_GunADS = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_GunADS"));
	FP_GunADS->SetOnlyOwnerSee(true);
	FP_GunADS->bCastDynamicShadow = false;
	FP_GunADS->CastShadow = false;
	FP_GunADS->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	initialHunger = 100.0f;
	currentHunger = initialHunger; 

	initialHealth = 100.0f;
	currentHealth = initialHealth;
}



void AMyProject2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	FP_GunADS->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	FP_GunADS->SetVisibility(false);

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	SupraTimer = 120;
}

void AMyProject2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SupraTimer -= 1;
	if (SupraTimer < 5) 
	{
		UpdateCurrentHunger(-DeltaTime * 0.001f * (initialHunger));
		SupraTimer = 120;

		

	}
	if (isFiring == true) 
	{
		FullyAutomaticFire();
	}
}
//******** STATS ********* STATS ********* STATS ********* STATS ********* STATS ********* STATS *********
//      
//*********************** These deal with health and interface *************
float AMyProject2Character::GetInitialHealth()
{
	return initialHealth;
}
float AMyProject2Character::GetCurrentHealth()
{
	return currentHealth;
}
void AMyProject2Character::UpdateCurrentHealth(float Health) 
{
	currentHealth = currentHealth + Health;
}
//*********************** These deal with hunger and interface *************
float AMyProject2Character::GetInitialHunger()
{
	return initialHunger;
}

float AMyProject2Character::GetCurrentHunger()
{
	return currentHunger;
}

void AMyProject2Character::UpdateCurrentHunger(float Hunger)
{
	currentHunger = currentHunger + Hunger;
	
}
//******** STATS END **** STATS END **** STATS END **** STATS END **** STATS END **** STATS END ***********
//*************************************************************************

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyProject2Character::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyProject2Character::StopFire);

	//Bind aim down sights event 
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMyProject2Character::OnAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMyProject2Character::OnHip);


	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyProject2Character::OnResetVR);

	//Bind PickUp Event
	PlayerInputComponent->BindAction("pickUp", IE_Pressed, this, &AMyProject2Character::onPickUp);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProject2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProject2Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyProject2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyProject2Character::LookUpAtRate);
}




void AMyProject2Character::onPickUp() 
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*200.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();



	//this is a raycast from the player character camera
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		//makes line
		//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);
		//makes message
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attempted Pickup: %s"), *HitResult->Actor->GetName()));

		AActor* TestTarget = Cast<AActor>(HitResult->Actor.Get());

		if (TestTarget != NULL && !TestTarget->IsPendingKill())
		{
			FOutputDeviceNull ar;
			TestTarget->CallFunctionByNameWithArguments(TEXT("pickupItem_Event_0"),ar,NULL,true);
		}
	}
}


//****************************** These are all firearm and or combat related functions *****************************************

//These 2 functions handle aiming
void AMyProject2Character::OnAim()
{
	FirstPersonCameraComponent->SetFieldOfView(70.0f);
	FP_Gun->SetVisibility(false);
	FP_GunADS->SetVisibility(true);
}

void AMyProject2Character::OnHip()
{
	FirstPersonCameraComponent->SetFieldOfView(90.0f);
	FP_Gun->SetVisibility(true);
	FP_GunADS->SetVisibility(false);
}


//end



//This is called by equipping a different weapon, and changes valued use by the firing mechanism 
//This function references meshes set in editor and sets up their relative transforms 
//this replaces FP_Gun, which is already attached to the player's animated skeleton mesh 
//LocY - left and right; Loc x - forward and backward; Loc z - up and down ;;;; rot yaw - left right ; rot pitch - lean left right ; rot roll - tip up down
void AMyProject2Character::changeGunEquipped(int gunNumber, int weaponType, int newrateOfFire, int gunDamage, int magMax, int roundsInMag)
{
	//This sets values used by the firing function
	currentFireType				 = weaponType;
	rateOfFire                   = newrateOfFire;
	gunBaseDamage                = gunDamage;
	currentWeaponMagazineMaximum = magMax;
	currentGunNumber             = gunNumber;
	ammoInMagazine               = roundsInMag;

	if (gunNumber == 0) //revolver
	{
		//**********hip gun***********
		FP_Gun->SetSkeletalMesh(Revolver);
		FRotator Rotation;
		Rotation.Yaw   = 50; //z axis rotation
		Rotation.Pitch = 20;  //y axis rotation
		Rotation.Roll  = 0;  //x axis rotation
		FVector Scale;
		Scale.X        = .2;
		Scale.Y        = .2;
		Scale.Z        = .2;
		FVector Location;
		Location.X = 20.0;
		Location.Y = -5.0;
		Location.Z = 8;
		     //set the gun mesh here
		FP_Gun->SetRelativeRotation(Rotation);
		FP_Gun->SetWorldScale3D(Scale);
		FP_Gun->SetRelativeLocation(Location);
		//**********iron sights*******
		FP_GunADS->SetSkeletalMesh(Revolver);
		FRotator Rotation2;
		Rotation2.Yaw = 50;
		Rotation2.Pitch = 20;
		Rotation2.Roll = 0;
		FVector Location2;
		Location2.X = 20;
		Location2.Y = -5;
		Location2.Z = 15;
		FP_GunADS->SetRelativeRotation(Rotation2);
		FP_GunADS->SetWorldScale3D(Scale);
		FP_GunADS->SetRelativeLocation(Location2);


	}
	else if (gunNumber == 1) //AK103
	{
		FP_Gun->SetSkeletalMesh(AK103);
		FRotator Rotation;
		Rotation.Yaw = 53; //z axis rotation
		Rotation.Pitch = 13;  //y axis rotation
		Rotation.Roll = -10;  //x axis rotation
		FVector Scale;
		Scale.X = 2;
		Scale.Y = 2;
		Scale.Z = 2;
		FVector Location;
		Location.X = 15.0;
		Location.Y = 10.0;
		Location.Z = -25;
		     //set the gun mesh here
		FP_Gun->SetRelativeRotation(Rotation);
		FP_Gun->SetWorldScale3D(Scale);
		FP_Gun->SetRelativeLocation(Location);
		//**********iron sights*******
		FP_GunADS->SetSkeletalMesh(AK103);
		FRotator Rotation2;
		Rotation2.Yaw = 55;
		Rotation2.Pitch = 13;
		Rotation2.Roll = -7;
		FVector Location2;
		Location2.X = 28;
		Location2.Y = 13.5;
		Location2.Z = -28;
		FP_GunADS->SetRelativeRotation(Rotation2);
		FP_GunADS->SetWorldScale3D(Scale);
		FP_GunADS->SetRelativeLocation(Location2);
	}
	else if (gunNumber == 2) //NavalRevolver
	{
		FP_Gun->SetSkeletalMesh(NavalRevolver);
		FRotator Rotation;
		Rotation.Yaw = 150; //z axis rotation
		Rotation.Pitch = 25;  //y axis rotation
		Rotation.Roll = 15;  //x axis rotation
		FVector Scale;
		Scale.X = .065;
		Scale.Y = .065;
		Scale.Z = .065;
		FVector Location;
		Location.X = 5.0;
		Location.Y = -7.0;
		Location.Z = 8;
		//set the gun mesh here
		FP_Gun->SetRelativeRotation(Rotation);
		FP_Gun->SetWorldScale3D(Scale);
		FP_Gun->SetRelativeLocation(Location);
		//**********iron sights*******
		FP_GunADS->SetSkeletalMesh(NavalRevolver);
		FRotator Rotation2;
		Rotation2.Yaw = 150;
		Rotation2.Pitch = 25;
		Rotation2.Roll = 15;
		FVector Location2;
		Location2.X = 5;
		Location2.Y = -7;
		Location2.Z = 15;
		FP_GunADS->SetRelativeRotation(Rotation2);
		FP_GunADS->SetWorldScale3D(Scale);
		FP_GunADS->SetRelativeLocation(Location2);
	}
	else if (gunNumber == 3) //Plains Percussion Rifle
	{
		FP_Gun->SetSkeletalMesh(PlainsPercussionRifle);
		FRotator Rotation;
		Rotation.Yaw = 143; //z axis rotation
		Rotation.Pitch = 10;  //y axis rotation
		Rotation.Roll = 15;  //x axis rotation
		FVector Scale;
		Scale.X = 1.2;
		Scale.Y = 1.2;
		Scale.Z = 1.2;
		FVector Location;
		Location.X = 5.0;
		Location.Y = -9.0;
		Location.Z = 8;
		//set the gun mesh here
		FP_Gun->SetRelativeRotation(Rotation);
		FP_Gun->SetWorldScale3D(Scale);
		FP_Gun->SetRelativeLocation(Location);
		//**********iron sights*******
		FP_GunADS->SetSkeletalMesh(PlainsPercussionRifle);
		FRotator Rotation2;
		Rotation2.Yaw = 143;
		Rotation2.Pitch = 10;
		Rotation2.Roll = 15;
		FVector Location2;
		Location2.X = 5;
		Location2.Y = -9;
		Location2.Z = 8;
		FP_GunADS->SetRelativeRotation(Rotation2);
		FP_GunADS->SetWorldScale3D(Scale);
		FP_GunADS->SetRelativeLocation(Location2);
	}
	

}
void AMyProject2Character::StopFire() 
{
	isFiring = false;
}
void AMyProject2Character::OnFire()
{
	
	//check for ammo and gun type to determine rate of fire
	if (ammoInMagazine > 0) 
	{
		if (currentFireType == 0) //semi-automatic
		{
			SemiAutomaticFire();
		}
		else if (currentFireType == 1) //fully automatic
		{
			firearmTimer = 0;
			isFiring = true;
		} 
	}
}
void AMyProject2Character::DoRayCast()
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*20000.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();



	//this is a raycast from the player character camera
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		//makes line
		//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);
		//makes message
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));

		AMyEnemyBase* TestTarget = Cast<AMyEnemyBase>(HitResult->Actor.Get());

		if (TestTarget != NULL && !TestTarget->IsPendingKill())
		{
			TestTarget->DamageTarget(gunBaseDamage);
		}

		FVector_NetQuantizeNormal Locs;
		FRotator Rots;
		Locs = HitResult->Location;
		SpawnBulletImpact(Locs, Rots);


	}
	

}

void AMyProject2Character::SpawnGunSmoke() 
{
	FRotator SpawnRotation;
	SpawnRotation = GetControlRotation();

	FVector x;
	x = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
	

	FActorSpawnParameters SpawnParams;
	AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(GunSmoke, x, SpawnRotation, SpawnParams);
}

void AMyProject2Character::SpawnBulletImpact(FVector Loc, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(BulletImpact, Loc, Rot, SpawnParams);


}
void AMyProject2Character::FireSoundAndAnimation() 
{

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AMyProject2Character::SemiAutomaticFire() 
{
	
	DoRayCast();
	FireSoundAndAnimation();
	SpawnGunSmoke();
	ammoInMagazine -= 1;
	
}
void AMyProject2Character::FullyAutomaticFire()
{
	firearmTimer -= 1;
	if (firearmTimer < 5) 
	{
		if (ammoInMagazine > 0) 
		{
			DoRayCast();
			FireSoundAndAnimation();
			SpawnGunSmoke();
			ammoInMagazine -= 1;

			firearmTimer = rateOfFire;
		}
	}
}

//*****************************************************************************************************************************

void AMyProject2Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyProject2Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AMyProject2Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AMyProject2Character::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

// MOVEMENT RELATED ************************************************************************************************************
void AMyProject2Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyProject2Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMyProject2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProject2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AMyProject2Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMyProject2Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AMyProject2Character::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMyProject2Character::TouchUpdate);
		return true;
	}
	
	return false;
}
