// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ItemPickup.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "enemybaseClass.h"
#include "GameFramework/InputSettings.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//TODO : Make individual componenets for gun magazines, silenceers, and other attachments;;
//TODO : Make 2 arm meshcomponents to attach to the gun object, and accompanying transform values
//////////////////////////////////////////////////////////////////////////////////////////////////


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	//Create an inventory
	FRotator Rots;
	FVector Loc = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	//Inventory = GetWorld()->SpawnActor<AInventory>(InventoryDefault, Loc, Rots, SpawnParams);
	//As this occassionally stops constructing at runtime, the bluepirnt spawns one successfully and sets characters ref to it
	Inventory = CreateDefaultSubobject<AInventory>(TEXT("PlayerInventory"));

	//Create a BodyMesh
	MeshBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	MeshBody->bCastDynamicShadow = true;
	MeshBody->CastShadow = true;
	MeshBody->SetupAttachment(RootComponent);

	//Create a Torso Mesh
	MeshTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TorsoArmor"));
	MeshTorso->bCastDynamicShadow = true;
	MeshTorso->CastShadow = true;
	MeshTorso->SetupAttachment(RootComponent);

	//Create Arms - no longer necessary, however the previously determined firearm locations are affected by this. easier to keep. 
	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->bCastDynamicShadow = true;
	Arms->CastShadow = true;
	Arms->SetupAttachment(FirstPersonCameraComponent);

	//Gun 
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunWithArms"));
	Gun->bCastDynamicShadow = true;
	Gun->CastShadow = true;
	Gun->SetupAttachment(Arms);

	//Arms
	LeftArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftArm"));
	LeftArm->bCastDynamicShadow = true;
	LeftArm->CastShadow = true;
	LeftArm->SetupAttachment(Gun);

	RightArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightArm"));
	RightArm->bCastDynamicShadow = true;
	RightArm->CastShadow = true;
	RightArm->SetupAttachment(Gun);

	//Magazine
	MeshMagazine = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Magazine"));
	MeshMagazine->bCastDynamicShadow = true;
	MeshMagazine->CastShadow = true;
	MeshMagazine->SetupAttachment(Gun, TEXT("SOCK"));
	
	//MeshMag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mag"));
	//MeshMag->bCastDynamicShadow = true;
	//MeshMag->CastShadow = true;
	//MeshMag->AttachToComponent(Gun, "SOCK");

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	RelativeGunlocation.Set(0.0f,0.0f,0.0f);
	AdsRelativeGunLocation.Set(0.0f, 0.0f, 0.0f);
	GunRotation;
	AdsGunRotation;
	GunScale.Set(0.0f, 0.0f, 0.0f);
	gunBaseDamage          = 0;
	ammoInMagazine         = 0;
	currentFireType        = 0;
	isFiring               = false;
	GunOffset.Set(100.0f, 0.0f, 10.0f);

	RelativeMagazineLocation.Set(0.0f, 0.0f, 0.0f);
	AdsRelativeMagazineLocation.Set(0.0f, 0.0f, 0.0f);
	MagazineRotation;AdsMagazineRotation;
	MagazineScale.Set(0.0f, 0.0f, 0.0f);

	//Set to false 
	isADS = false;

	rateOfFire = 0.0f;

	HEALTH = 1; STAMINA = 1; HUNGER = 1; WATER = 1; FATIGUE = 1;

	pausecheck = 1;
	Paused = false;
	CurrentEquippedWeapon = 6666;
	GenerateWeaponText();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyCharacter::Interact);

	PlayerInputComponent->BindAction("OnFire", IE_Pressed, this, &AMyCharacter::OnFire);
	PlayerInputComponent->BindAction("OnFire", IE_Released, this, &AMyCharacter::ReleaseTrigger);

	PlayerInputComponent->BindAction("AdsIn", IE_Pressed, this, &AMyCharacter::AimDownSights);
	PlayerInputComponent->BindAction("AdsIn", IE_Released, this, &AMyCharacter::RelaxAim);

	PlayerInputComponent->BindAction("PressE", IE_Pressed, this, &AMyCharacter::PauseCheck);

	//Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	

}

void AMyCharacter::PauseCheck()
{
	pausecheck += 1;
	if (pausecheck % 2 == 0) { Paused = true;  }
	if (pausecheck % 2 != 0) { Paused = false; }
}

void AMyCharacter::SetHoursMinutes(int H, int M)
{
	Hour = H;
	Minutes = M;
	GenerateClockText();
}

//called by the widget ui and used to display the current game time
void AMyCharacter::GenerateClockText()
{
	FString HourString = ""; FString M = "";
	if (Hour < 10) { HourString = "0" + FString::FromInt(Hour); M = "A.M."; }
	if (Hour >= 10) { if (Hour < 13) { HourString = FString::FromInt(Hour); M = "A.M."; } }
	if (Hour >= 13) { if (Hour < 22) { HourString = "0" + FString::FromInt(Hour - 12); M = "P.M."; } }
	if (Hour >= 22) { HourString = FString::FromInt(Hour); M = "P.M."; }

	ClockText = ClockText + ":";
	//01:, 11:, etc
	if (Minutes > 9) { ClockText = HourString + ":" + FString::FromInt(Minutes) + " " + M; }
	if (Minutes < 9) { ClockText = HourString + ":" + "0" + FString::FromInt(Minutes) + " " + M; }
	//01:13, 13:17

	UpdateStats();
}

//gradual increase of health and decrease of stats
void AMyCharacter::UpdateStats()
{
	//These numbers are not arbitrary. They represent 3 days without water, 3 weeks without food, and 11 days without sleep
	WATER  -= .0002315;
	HUNGER -= .000033;
	FATIGUE -= .000138;

	if (HEALTH < 1) { HEALTH += .0003f; }
	if (STAMINA < 1) { STAMINA += .01f; }
	if (HUNGER < .20f) { HEALTH -= .0001; }
	if (HUNGER < .01f) { HEALTH -= 0.005F; }
	if (WATER < .40f) { HEALTH -= .0003f; }
	if (WATER < .01f) { HEALTH -= .02f; }
	if (FATIGUE < .60f) { HEALTH -= .0003f; }
	if (FATIGUE < .01f) { HEALTH -= .02f; }

}


//Aiming
void AMyCharacter::AimDownSights()
{
	//manipulate gun locations
	Gun->SetRelativeLocation(AdsRelativeGunLocation);
	Gun->SetRelativeRotation(AdsGunRotation);

	//send signal for movement blueprint
	FName AnimPropName = TEXT("isADS");
	if (UAnimInstance* AnimInst = Gun->GetAnimInstance())
	{
		UBoolProperty* myBool = FindField<UBoolProperty>(AnimInst->GetClass(), AnimPropName);
		if (myBool != NULL)
		{
			myBool->SetPropertyValue_InContainer(AnimInst, true);
		}
	}
	//keep for reference
	FirstPersonCameraComponent->SetFieldOfView(85.0f);
	isADS = true;
}

void AMyCharacter::RelaxAim()
{
	Gun->SetRelativeLocation(RelativeGunlocation);
	Gun->SetRelativeRotation(GunRotation);

	//send signal for movement blueprint
	FName AnimPropName = TEXT("isADS");
	if (UAnimInstance* AnimInst = Gun->GetAnimInstance())
	{
		UBoolProperty* myBool = FindField<UBoolProperty>(AnimInst->GetClass(), AnimPropName);
		if (myBool != NULL)
		{
			myBool->SetPropertyValue_InContainer(AnimInst, false);
		}
	}
	FirstPersonCameraComponent->SetFieldOfView(90.0f);
	isADS = false;
}

//Game World Interactions
void AMyCharacter::Interact()
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*1000.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		AItemPickup* TestTarget = Cast<AItemPickup>(HitResult->Actor.Get());

		if (TestTarget != NULL && !TestTarget->IsPendingKill())
		{
			//pick up item
			int ItemID = TestTarget->PickUpItem();
			int ammu   = TestTarget->GetAmmo();
			bool cons  = TestTarget->GetConsum();
			bool equip = TestTarget->GetWep();
			bool res   = TestTarget->GetRes();
			bool cont  = TestTarget->GetCont();

			//if (ItemID != NULL) 
			//{
				
				Inventory->AddToInventory(ItemID, ammu, cons, equip, res, cont);

				//play pickup sound
				if (PickupSound != NULL)
				{
					UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
				}

				TestTarget->Destroy();
			//}
		}
	}
}

// MOVEMENT RELATED ************************************************************************************************************
void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);

		
			//Set gun anim BP to walk
			FName AnimPropName = TEXT("isWalkingForward");
			if (UAnimInstance* AnimInst = Gun->GetAnimInstance())
			{
				UBoolProperty* myBool = FindField<UBoolProperty>(AnimInst->GetClass(), AnimPropName);
				if (myBool != NULL)
				{
					myBool->SetPropertyValue_InContainer(AnimInst, true);
				}
			}
		

	}

	if (Value == 0.0f)
	{
		//Set gun anim BP to idle
		FName AnimPropName = TEXT("isWalkingForward");
		if (UAnimInstance* AnimInst = Gun->GetAnimInstance())
		{
			UBoolProperty* myBool = FindField<UBoolProperty>(AnimInst->GetClass(), AnimPropName);
			if (myBool != NULL)
			{
				myBool->SetPropertyValue_InContainer(AnimInst, false);
			}
		}
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);

		
			//Set gun anim BP to walk
			FName AnimPropName = TEXT("isWalkingRight");
			if (UAnimInstance* AnimInst = Gun->GetAnimInstance())
			{
				UBoolProperty* myBool = FindField<UBoolProperty>(AnimInst->GetClass(), AnimPropName);
				if (myBool != NULL)
				{
					myBool->SetPropertyValue_InContainer(AnimInst, true);
				}
			}
		
	}

	if (Value == 0.0f)
	{
		
		//Set gun anim BP to idle
		FName AnimPropName = TEXT("isWalkingRight");
		if (UAnimInstance* AnimInst = Gun->GetAnimInstance())
		{
			UBoolProperty* myBool = FindField<UBoolProperty>(AnimInst->GetClass(), AnimPropName);
			if (myBool != NULL)
			{
				myBool->SetPropertyValue_InContainer(AnimInst, false);
			}
		}
	}
}



//Guns
void AMyCharacter::SetAmmoInMag(int itemID)
{
	ammoInMagazine = itemID;
}

void AMyCharacter::ChangeGunEquipped(int gunNumber)
{
	//set the mesh
	switch (gunNumber) 
	{
	case 0: Gun->SetSkeletalMesh(Gun0MeshReference); SetGunVariables(0);
			Gun->SetAnimClass(Gun0AnimReference->GetAnimBlueprintGeneratedClass());
			FireAnimation = Gun0FireAnimation;
			currentFireType = 0; 
		break;

	case 1: Gun->SetSkeletalMesh(Gun1MeshReference); MeshMagazine->SetSkeletalMesh(Gun1MagazineMeshReference); SetGunVariables(1);
			Gun->SetAnimClass(Gun1AnimReference->GetAnimBlueprintGeneratedClass());
			MeshMagazine->SetupAttachment(Gun, TEXT("SOCK"));
			FireAnimation = Gun1FireAnimation;
			currentFireType = 1; rateOfFire = 0.08f; 
		break;

	case 3: Gun->SetSkeletalMesh(Gun3MeshReference); SetGunVariables(3);
			Gun->SetAnimClass(Gun3AnimReference->GetAnimBlueprintGeneratedClass());
			FireAnimation = Gun3FireAnimation;
			currentFireType = 1; rateOfFire = 0.06f; 
		break;

	case 7: Gun->SetSkeletalMesh(Gun7MeshReference); MeshMagazine->SetSkeletalMesh(Gun7WoodenParts); SetGunVariables(7);
			Gun->SetAnimClass(Gun7AnimReference->GetAnimBlueprintGeneratedClass());
			FireAnimation = Gun7FireAnimation;
			currentFireType = 1; rateOfFire = 0.07f;
		break;

	case 8: Gun->SetSkeletalMesh(Gun8MeshReference); SetGunVariables(8);
			Gun->SetAnimClass(Gun8AnimReference->GetAnimBlueprintGeneratedClass());
			FireAnimation = Gun8FireAnimation;
			currentFireType = 2;
	}

	CurrentEquippedWeapon = gunNumber;

	
}


void AMyCharacter::SetGunVariables(int gunNumber)
{
	//set location, adslocation, rotation, ads rotation, scale, gunbasedamage, rateoffire
	//should be handled by loading text files into array of array at start and referencing during runtime
	
	switch (gunNumber)
	{
	case 0:
		RelativeGunlocation.Set(-6.666f, 27.0f, 154.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 90.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = .2f; GunScale.Y = .2f; GunScale.Z = .2f; Gun->SetRelativeScale3D(GunScale);

		MagazineScale.X = 0.0f; MagazineScale.Y = 0.0f; MagazineScale.Z = 0.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);

		AdsRelativeGunLocation.X = 0.076412f; AdsRelativeGunLocation.Y = 35.255905f; AdsRelativeGunLocation.Z = 154.0f;
		AdsGunRotation.Roll = -0.000001f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 90.000008f;

		gunBaseDamage = 40;
		break;
	case 1:
		RelativeGunlocation.Set(-10.0f, 56.0f, 123.0f); Gun->SetRelativeLocation(RelativeGunlocation);

		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 90.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 1.8f; GunScale.Y = 1.8f; GunScale.Z = 1.8f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = 270.f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 0.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 0.01f; MagazineScale.Y = 0.01f; MagazineScale.Z = 0.01f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 0.0f; RelativeMagazineLocation.Y = 0.152f; RelativeMagazineLocation.Z = 0.0f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = 0.03f; AdsRelativeGunLocation.Y = 15.924101f; AdsRelativeGunLocation.Z = 120.5f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 89.999f;
		

		gunBaseDamage = 20;
		break;
	case 3:
		RelativeGunlocation.Set(-9.0f, 80.0f, 125.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 0.956615f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 1.0f; GunScale.Y = 1.0f; GunScale.Z = 1.0f; Gun->SetRelativeScale3D(GunScale);

		MagazineScale.X = 0.0f; MagazineScale.Y = 0.0f; MagazineScale.Z = 0.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);

		AdsRelativeGunLocation.X = 2.229065f; AdsRelativeGunLocation.Y = 47.279953f; AdsRelativeGunLocation.Z = 125.199997f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 0.0f;

		gunBaseDamage = 10;
		break;

	case 7:
		RelativeGunlocation.Set(-119.0f, 79.0f, 145.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 90.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 0.15f; GunScale.Y = 0.15f; GunScale.Z = 0.15f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = 0.0f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 270.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 0.175f; MagazineScale.Y = 0.175f; MagazineScale.Z = 0.175f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 120.4985f; RelativeMagazineLocation.Y = 28.1396f; RelativeMagazineLocation.Z = -19.056f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = -104.7f; AdsRelativeGunLocation.Y = 27.0f; AdsRelativeGunLocation.Z = 145.0f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 90.0f;

		gunBaseDamage = 17;
		break;

	case 8: break;
	}
}

void AMyCharacter::FireWeaponOrTool()
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*20000.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	//play firing animation
	UAnimInstance* AnimInstance = Gun->GetAnimInstance();
	if (AnimInstance != NULL)
	{
		FName AnimPropName = TEXT("isWalkingForward");
		if (UAnimInstance* AnimInst = Gun->GetAnimInstance())
		{
			UBoolProperty* myBool = FindField<UBoolProperty>(AnimInst->GetClass(), AnimPropName);
			if (myBool != NULL)
			{
				myBool->SetPropertyValue_InContainer(AnimInst, false);
			}
		}

		FName AnimPropName2 = TEXT("isWalkingRight");
		if (UAnimInstance* AnimInst = Gun->GetAnimInstance())
		{
			UBoolProperty* myBool = FindField<UBoolProperty>(AnimInst->GetClass(), AnimPropName);
			if (myBool != NULL)
			{
				myBool->SetPropertyValue_InContainer(AnimInst, false);
			}
		}

		AnimInstance->Montage_Play(FireAnimation, 1.f);
	}

	//spawn gun flash
	FRotator xx; FVector zz = FP_MuzzleLocation->GetComponentLocation(); FActorSpawnParameters SpawnParams;
	AActor* GunFlashSpawned = GetWorld()->SpawnActor<AActor>(GunFlash, zz, xx, SpawnParams);

	if (ammoInMagazine > 0) {
		//this is a raycast from the player character camera
		if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
		{
			//makes line
			//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);
			//makes message
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));

			AenemyBaseClass* TestTarget = Cast<AenemyBaseClass>(HitResult->Actor.Get());

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

}

void AMyCharacter::SpawnBulletImpact(FVector Loc, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(BulletImpact, Loc, Rot, SpawnParams);
}

void AMyCharacter::SemiAutomaticFire()
{

	FireWeaponOrTool();
	//FireSoundAndAnimation();
	SpawnGunSmoke();
	ammoInMagazine -= 1;
	GenerateWeaponText();

}
void AMyCharacter::FullyAutomaticFire()
{
	if (isFiring == true) {

		
		FireWeaponOrTool();
		//FireSoundAndAnimation();
		SpawnGunSmoke();
		ammoInMagazine -= 1;
		GenerateWeaponText();
		GetWorld()->GetTimerManager().SetTimer(firingPin, this, &AMyCharacter::FullyAutomaticFire, rateOfFire, false);
			
	}
}

void AMyCharacter::SpawnGunSmoke()
{
	FRotator SpawnRotation;FVector x; FActorSpawnParameters SpawnParams;
	
	x = Gun->GetSocketLocation(TEXT("MUZZLE"));

	AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(GunSmoke, x, SpawnRotation, SpawnParams);
	FVector Scale = FVector(.2f, .2f, .2f);
	SPawnedActorRef->SetActorScale3D(Scale);
}

void AMyCharacter::OnFire()
{
	isFiring = true;
	//check for ammo and gun type to determine rate of fire
	//if (ammoInMagazine > 0)
	//{
	//	if (currentFireType == 0) //semi-automatic
	//	{
	//		SemiAutomaticFire();
	//	}
	//	else if (currentFireType == 1) //fully automatic
	//	{
	//		FullyAutomaticFire();
	//	}
	//}

	switch (currentFireType)
	{
	case 0: if (ammoInMagazine > 0) {SemiAutomaticFire(); } break;
	case 1: if (ammoInMagazine > 0) {FullyAutomaticFire();} break;
	case 2: GatherWater(); break;
	}
}

//This is called when the player clicks the LMB while holding any container capable of holding water
void AMyCharacter::GatherWater()
{

	//See if player is appropriately close to a body of water
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*20000.f) + StartTrace);
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		AActor* TestTarget = Cast<AActor>(HitResult->Actor.Get());

		if (TestTarget != NULL && !TestTarget->IsPendingKill())
		{
			FString x = TestTarget->GetName();

			//bodies of water are named "Water" + n
			if (x.Contains("Water"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));
				//we refer to both the slot in the inventory where our container resides, and 1, the identifier for fresh water in the context of the inventory context array
				Inventory->SetContainerContents(InventoryIndexEquipped, 1); Inventory->SetContainerAmount(InventoryIndexEquipped, 1);
			}
		}
	}
	//perform animation and play sound

	//set contents variable in inventory and in item - though we have stablished passing the information to the invntory, we must still pass it back to the item slot in the UI WITHOUT forcing a manual refresh of the UI by the user

	//set contents amount 
}

void AMyCharacter::ReleaseTrigger(){ isFiring = false; }

void AMyCharacter::GenerateWeaponText()
{
	FString Caliber = Inventory->GetWeaponCaliber(CurrentEquippedWeapon);

	WeaponText = FString::FromInt(ammoInMagazine) + "/" + "0" + " " + "[ " + Caliber + " ]";
}

//Initially created for the "water gathering" mechanic, this is used to store where in the inventory the player's current tool or weapon resides.
//Why? This way, when something like water gathering occurs, we can set the arrays in the inventory refering to item contents and amounts at the appropriate index
//EG: the inventory may hold value [8] in a slot 7 of the item ID array. as 8 refer to plastic containers, when we gather water, we can set slot 7 of the contents array to "unboiled water."
void AMyCharacter::ChangeItemIndexEquipped(int IndexNumber)
{
	InventoryIndexEquipped = IndexNumber;
}

