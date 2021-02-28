// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ItemPickup.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "PlantObject.h"
#include "FarmActor.h"
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
	SeedEquipped = 0;
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

	//Intermediary
	Intermediary = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RotationCorrection"));
	Intermediary->SetupAttachment(Arms);
	Intermediary->SetRelativeLocation(FVector(0.0f, 0.0f, 270.0f));

	//Gun 
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunWithArms"));
	Gun->bCastDynamicShadow = true;
	Gun->CastShadow = true;
	Gun->SetupAttachment(Intermediary);

	//Arms
	LeftArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftArm"));
	LeftArm->bCastDynamicShadow = true;
	LeftArm->CastShadow = true;
	LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));

	RightArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightArm"));
	RightArm->bCastDynamicShadow = true;
	RightArm->CastShadow = true;
	RightArm->SetupAttachment(Gun, TEXT("RightArm"));

	//Magazine
	MeshMagazine = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Magazine"));
	MeshMagazine->bCastDynamicShadow = true;
	MeshMagazine->CastShadow = true;
	MeshMagazine->SetupAttachment(Gun, TEXT("SOCK"));

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(Gun, TEXT("MUZZLE"));
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
	buildcheck = 1;
	Paused = false;
	CurrentEquippedWeapon = 6666;
	//GenerateWeaponText();
	walkspeed = 0.2f;

	SwaySpeed = 10.0f;

	IntermediaryLocation.Set(0.0f, 0.0f, 0.0f); ADSIntermediaryLocation.Set(0.0f, 0.0f, 0.0f);
	IndexSearching = 0;

	pitchReset = 0;

	meleeTimer = 0;
	PlantSeedSelected = 0;
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

	PlayerInputComponent->BindAction("PressQ", IE_Pressed, this, &AMyCharacter::PauseCheck);

	PlayerInputComponent->BindAction("PressY", IE_Pressed, this, &AMyCharacter::BuildMenu);

	//Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::NewTurnFunction);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::NewLookFunction);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::JumpStaminaDrain);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//running
	PlayerInputComponent->BindAction("LeftShift", IE_Pressed, this, &AMyCharacter::StartRunning);
	PlayerInputComponent->BindAction("LeftShift", IE_Released, this, &AMyCharacter::StopRunning);

	//scroll wheel for weapon switching
	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &AMyCharacter::ScrollWheelUp);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &AMyCharacter::ScrollWheelDown);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyCharacter::Reload);
	

}
//reloading
void AMyCharacter::Reload()
{
	int currentMagazineMax = Inventory->GetMagazineMax(Inventory->ItemsInInventory[InventoryIndexEquipped]);
	while (ammoInMagazine < currentMagazineMax && Inventory->GetAmmoAvailableForGivenWeapon(CurrentEquippedWeapon) > 0)
	{
		AddBullet();
	}
}
void AMyCharacter::AddBullet()
{
	ammoInMagazine += 1; Inventory->AmmoInWeapon[InventoryIndexEquipped] += 1;
	Inventory->SpendBullet(Inventory->GetItemIDForAmmoSpecificToWeaponItemID(CurrentEquippedWeapon));
	GenerateWeaponText();


}
//Weapon switching with scroll wheel
void AMyCharacter::ScrollWheelUp()
{
	IndexSearching = InventoryIndexEquipped;
	ScrollWheel(-1);
}
void AMyCharacter::ScrollWheelDown()
{
	IndexSearching = InventoryIndexEquipped;
	ScrollWheel(1);
}
void AMyCharacter::ScrollWheel(int dir)
{
	if (CurrentEquippedWeapon != 6666) {
		//1 = up, -1 = down
		IndexSearching = IndexSearching + dir;
		int indexWithWeapon = 0;

		if (IndexSearching <= 99 && IndexSearching > -1)
		{
			if (Inventory->isWeapon[IndexSearching] == true)
			{
				int gunNumber = Inventory->GetGunNumber(Inventory->ItemsInInventory[IndexSearching]);
				ChangeGunEquipped(gunNumber);
				SetAmmoInMag(Inventory->AmmoInWeapon[IndexSearching]);
				ChangeItemIndexEquipped(IndexSearching);
			}
			else { ScrollWheel(dir); }
		}
		//else if (IndexSearching > 99) { IndexSearching = -1; ScrollWheel(dir); }
		//else if (IndexSearching <= -1) { IndexSearching = 100; ScrollWheel(dir); }
		switch (IndexSearching)
		{
		case -1: IndexSearching = 100; ScrollWheel(dir); break;
		case 100: IndexSearching = -1; ScrollWheel(dir); break;
		}
	}

}

//Turning and Weapon Sway/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyCharacter::NewTurnFunction(float Rate) {
	AddControllerYawInput(Rate); LookVector.X = Rate; SwayScript(Rate); SwayScriptTwo(Rate); 
}
void AMyCharacter::NewLookFunction(float Rate) {
	AddControllerPitchInput(Rate); LookVector.Y = Rate; SwayScriptTwo(Rate);
}
void AMyCharacter::SwayScript(float Rate)
{
	FRotator newRot;
	newRot = Arms->GetRelativeRotation();
	float ZValue = newRot.Yaw;
	float newOffset = ZValue - Rate;
	Offset = newOffset;
	newRot.Yaw = Offset;
	Arms->SetRelativeRotation(newRot);

	newRot = Arms->GetRelativeRotation();
	FRotator newRot4 = newRot;
	newRot4.Yaw = FirstPersonCameraComponent->GetRelativeRotation().Yaw;

	FVector xVec1 = UKismetMathLibrary::Conv_RotatorToVector(newRot);
	FVector xVec2 = UKismetMathLibrary::Conv_RotatorToVector(newRot4);
	FVector calcu = xVec1 - xVec2;
	FVector norm = calcu.GetSafeNormal();
	float length = norm.Size();
	float speed = length * SwaySpeed;


	FRotator newRot2 = FMath::RInterpTo(newRot, newRot4, GetWorld()->GetDeltaSeconds(), speed);
	Arms->SetRelativeRotation(newRot2);
}

void AMyCharacter::SwayScriptTwo(float Rate)
{
	FVector newLoc = Gun->GetRelativeLocation();
	if (LookVector.X != 0.0f)
	{
		float moveX = -LookVector.X * 2.0f;
		float newY = FMath::FInterpTo(newLoc.Y, moveX, GetWorld()->GetDeltaSeconds(), 5.0f);
		newLoc.Y = newY;
		Gun->SetRelativeLocation(newLoc);
	}
	else
	{
		float newY = FMath::FInterpTo(newLoc.Y, 0.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
		newLoc.Y = newY;
		Gun->SetRelativeLocation(newLoc);
	}

	if (LookVector.Y != 0.0f)
	{
		float moveY = LookVector.Y * 2.0f;
		float newZ = FMath::FInterpTo(newLoc.Z, moveY, GetWorld()->GetDeltaSeconds(), 5.0f);
		newLoc.Z = newZ;
		Gun->SetRelativeLocation(newLoc);
	}
	else
	{
		float newZ = FMath::FInterpTo(newLoc.Z, 0.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
		newLoc.Z = newZ;
		Gun->SetRelativeLocation(newLoc);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AMyCharacter::StartRunning() { running = true; }
void AMyCharacter::StopRunning() { running = false; }

void AMyCharacter::JumpStaminaDrain()
{
	if (STAMINA > .05)
	{
		Jump();
		STAMINA -= .05;
		FATIGUE -= .0005;
	}
}

void AMyCharacter::BuildMenu()
{
	buildcheck += 1;
	if (buildcheck % 2 == 0) { OpenBuildMenu(); }
	if (buildcheck % 2 != 0) { CloseBuildMenu(); }
}


//setignorelookinput setignoremoveinput true/false
void AMyCharacter::PauseCheck()
{
	ResetMenuVisibility();
	pausecheck += 1;
	if (pausecheck % 2 == 0) { UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreLookInput(true); UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreMoveInput(true); Paused = true; }
	if (pausecheck % 2 != 0) { UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreLookInput(false); UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreMoveInput(false); Paused = false; }
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
	if (Minutes <= 9) { ClockText = HourString + ":" + "0" + FString::FromInt(Minutes) + " " + M; }
	//01:13, 13:17

	UpdateStats();
}

//gradual increase of health and decrease of stats
void AMyCharacter::UpdateStats()
{
	//These numbers are not arbitrary. They represent 3 days without water, 3 weeks without food, and 11 days without sleep
	WATER -= .0000266875;//.0002315;
	HUNGER -= .000004125;//.000033
	FATIGUE -= .00001725;//.000138     these were all divided by 8 as the time scale was changed

	if (HEALTH < 1) { HEALTH += .0003f; }
	if (STAMINA < 1) { STAMINA += .0005f; }
	if (HUNGER < .20f) { HEALTH -= .0001; }
	if (HUNGER < .01f) { HEALTH -= 0.005F; }
	if (WATER < .40f) { HEALTH -= .0003f; }
	if (WATER < .01f) { HEALTH -= .02f; }
	if (FATIGUE < .60f) { HEALTH -= .0003f; }
	if (FATIGUE < .01f) { HEALTH -= .02f; }
	if (running == true) { STAMINA -= .003f; FATIGUE -= .00002; }

}


//Aiming
void AMyCharacter::AimDownSights()
{
	//manipulate gun locations
	//Gun->SetRelativeLocation(AdsRelativeGunLocation);
	//Gun->SetRelativeRotation(AdsGunRotation);
	Intermediary->SetRelativeLocation(ADSIntermediaryLocation);
	

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
	//Gun->SetRelativeLocation(RelativeGunlocation);
	//Gun->SetRelativeRotation(GunRotation);
	Intermediary->SetRelativeLocation(IntermediaryLocation);

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
			bool stak  = TestTarget->GetStackable();
			bool isam = TestTarget->GetAmm();
			int QuestToAdd = TestTarget->GetNumberAdded();
			int QuestToComplete = TestTarget->GetNumberCompleted();
				
			Inventory->AddToInventory(ItemID, ammu, cons, equip, res, cont, stak, isam);

			//play pickup sound
			if (PickupSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
			}

			//initaite functions to check and see if picking up this item adds or completes any quests

			if (QuestToAdd      != 6666) { QuestManager->AddQuestByNumber(QuestToAdd);   }
			if (QuestToComplete != 6666) { QuestManager->CompleteQuest(QuestToComplete); }

			TestTarget->Destroy();
		}
	}
}

// MOVEMENT RELATED ************************************************************************************************************
void AMyCharacter::MoveForward(float Value)
{
	if (running == true) { if (STAMINA > 0.011f) { walkspeed = .4f; } else walkspeed = .2f; }
	else walkspeed = .2f;
	if (Value != 0.0f)
	{
		float x = Value * walkspeed;
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), x);

		
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
	if (running == true) { if (STAMINA > 0.011f) { walkspeed = .4f; } else walkspeed = .2f; }
	else walkspeed = .2f;
	if (Value != 0.0f)
	{
		float x = Value * walkspeed;
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), x);

		
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
	GenerateWeaponText();
}

void AMyCharacter::ChangeGunEquipped(int gunNumber)
{
	LeftArm->SetSkeletalMesh(LeftArmPart);
	RightArm->SetSkeletalMesh(RightArmPart);
	//set the mesh
	switch (gunNumber) 
	{
	case 0: Gun->SetSkeletalMesh(Gun0MeshReference); SetGunVariables(0);
			Gun->SetAnimClass(Gun0AnimReference->GetAnimBlueprintGeneratedClass());
			FireAnimation = Gun0FireAnimation;
			LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
			RightArm->SetupAttachment(Gun, TEXT("RightArm"));
			currentFireType = 0; 
		break;

	case 1: Gun->SetSkeletalMesh(Gun1MeshReference); MeshMagazine->SetSkeletalMesh(Gun1MagazineMeshReference); SetGunVariables(1);
			Gun->SetAnimClass(Gun1AnimReference->GetAnimBlueprintGeneratedClass());
			MeshMagazine->SetupAttachment(Gun, TEXT("SOCK"));
			FireAnimation = Gun1FireAnimation;
			LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
			RightArm->SetupAttachment(Gun, TEXT("RightArm"));
			currentFireType = 1; rateOfFire = 0.08f; 
		break;

	case 3: Gun->SetSkeletalMesh(Gun3MeshReference); SetGunVariables(3);
			Gun->SetAnimClass(Gun3AnimReference->GetAnimBlueprintGeneratedClass());
			FireAnimation = Gun3FireAnimation;
			LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
			RightArm->SetupAttachment(Gun, TEXT("RightArm")); 
			currentFireType = 1; rateOfFire = 0.06f; 
		break;

	case 4: Gun->SetSkeletalMesh(Gun4MeshReference); SetGunVariables(4);
		Gun->SetAnimClass(Gun4AnimReference->GetAnimBlueprintGeneratedClass());
		FireAnimation = Gun4FireAnimation;
		LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
		RightArm->SetupAttachment(Gun, TEXT("RightArm"));
		currentFireType = 4; rateOfFire = 5.0f;
		break;

	case 7: Gun->SetSkeletalMesh(Gun7MeshReference); MeshMagazine->SetSkeletalMesh(Gun7WoodenParts); SetGunVariables(7);
			Gun->SetAnimClass(Gun7AnimReference->GetAnimBlueprintGeneratedClass());
			FireAnimation = Gun7FireAnimation;
			LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
			RightArm->SetupAttachment(Gun, TEXT("RightArm"));
			currentFireType = 1; rateOfFire = 0.08f;
		break;

	case 8: Gun->SetSkeletalMesh(Gun8MeshReference); SetGunVariables(8);
			Gun->SetAnimClass(Gun8AnimReference->GetAnimBlueprintGeneratedClass());
			FireAnimation = Gun8FireAnimation;
			currentFireType = 2;
			break;

	case 13: Gun->SetSkeletalMesh(Gun10MeshReference); SetGunVariables(13);
		Gun->SetAnimClass(Gun10AnimReference->GetAnimBlueprintGeneratedClass());
		FireAnimation = Gun10FireAnimation;
		LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
		RightArm->SetupAttachment(Gun, TEXT("RightArm"));
		currentFireType = 1; rateOfFire = 0.07f;
		break;

	case 20: Gun->SetSkeletalMesh(Gun20MeshReference); MeshMagazine->SetSkeletalMesh(Gun20AxeHead); SetGunVariables(20);
		Gun->SetAnimClass(Gun20AnimReference->GetAnimBlueprintGeneratedClass());
		MeshMagazine->SetupAttachment(Gun, TEXT("SOCK"));
		FireAnimation = Gun20FireAnimation;
		LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
		RightArm->SetupAttachment(Gun, TEXT("RightArm"));
		currentFireType = 3; rateOfFire = 5.0f;
		break;

	case 24: Gun->SetSkeletalMesh(Gun24MeshReference); SetGunVariables(24);
		Gun->SetAnimClass(Gun24AnimReference->GetAnimBlueprintGeneratedClass());
		FireAnimation = Gun24FireAnimation;
		LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
		RightArm->SetupAttachment(Gun, TEXT("RightArm"));
		currentFireType = 0;
		break;

	case 25: Gun->SetSkeletalMesh(Gun25MeshReference); SetGunVariables(25);
		Gun->SetAnimClass(Gun25AnimReference->GetAnimBlueprintGeneratedClass());
		FireAnimation = Gun25FireAnimation;
		LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
		RightArm->SetupAttachment(Gun, TEXT("RightArm"));
		currentFireType = 0;
		break;

	case 26: Gun->SetSkeletalMesh(Gun26MeshReference); SetGunVariables(26);
		Gun->SetAnimClass(Gun26AnimReference->GetAnimBlueprintGeneratedClass());
		FireAnimation = Gun26FireAnimation;
		LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
		RightArm->SetupAttachment(Gun, TEXT("RightArm"));
		currentFireType = 0;
		break;

	case 28: Gun->SetSkeletalMesh(Gun28MeshReference); SetGunVariables(28);
		Gun->SetAnimClass(Gun28AnimReference->GetAnimBlueprintGeneratedClass());
		FireAnimation = Gun28FireAnimation;
		LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
		RightArm->SetupAttachment(Gun, TEXT("RightArm"));
		currentFireType = 1; rateOfFire = 0.08f;
		break;

		//potato seed
	case 30: Gun->SetSkeletalMesh(Gun30MeshReference); SetGunVariables(30);
		Gun->SetAnimClass(Gun30AnimReference->GetAnimBlueprintGeneratedClass());
		FireAnimation = Gun30FireAnimation;
		LeftArm->SetupAttachment(Gun, TEXT("LeftArm"));
		RightArm->SetupAttachment(Gun, TEXT("RightArm"));
		currentFireType = 5; rateOfFire = 0.08f; PlantSeedSelected = 31;
		break;


	}

	CurrentEquippedWeapon = gunNumber;
	GenerateWeaponText();
	
}


void AMyCharacter::SetGunVariables(int gunNumber)
{
	//set location, adslocation, rotation, ads rotation, scale, gunbasedamage, rateoffire
	//should be handled by loading text files into array of array at start and referencing during runtime

	switch (gunNumber)
	{
	case 0:
		//RelativeGunlocation.Set(-3.0f, 43.0f, 150.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 88.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = .2f; GunScale.Y = .2f; GunScale.Z = .2f; Gun->SetRelativeScale3D(GunScale);

		MagazineScale.X = 0.0f; MagazineScale.Y = 0.0f; MagazineScale.Z = 0.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);

		AdsRelativeGunLocation.X = 0.076412f; AdsRelativeGunLocation.Y = 43.0f; AdsRelativeGunLocation.Z = 154.0f;
		AdsGunRotation.Roll = -0.000001f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 90.000008f;

		//arms
		ArmScale.X = .05f; ArmScale.Y = .05f; ArmScale.Z = .05f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);
		RelativeLeftArmLocation.X = -1.387729f; RelativeLeftArmLocation.Y = -0.004257f; RelativeLeftArmLocation.Z = 0.087819f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = -0.710680f; RelativeLeftArmRotation.Yaw = 98.032173f; RelativeLeftArmRotation.Roll = 82.647003f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = -1.447519f; RelativeRightArmLocation.Y = 0.149462f; RelativeRightArmLocation.Z = 0.451750f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = 11.960870f; RelativeRightArmRotation.Yaw = 84.421829f; RelativeRightArmRotation.Roll = 78.219543f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(61.0f, -9.0f, 145.0f); ADSIntermediaryLocation.Set(49.0f, -9.0f, 150.0f);

		gunBaseDamage = 40;
		break;
	case 1:
		//RelativeGunlocation.Set(-14.0f, 85.0f, 116.0f); Gun->SetRelativeLocation(RelativeGunlocation);

		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 90.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 1.8f; GunScale.Y = 1.8f; GunScale.Z = 1.8f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = 270.f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 0.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 0.01f; MagazineScale.Y = 0.01f; MagazineScale.Z = 0.01f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 0.0f; RelativeMagazineLocation.Y = 0.152f; RelativeMagazineLocation.Z = 0.0f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = 0.03f; AdsRelativeGunLocation.Y = 103.0f; AdsRelativeGunLocation.Z = 120.5f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 89.999f;
		
		//arms
		ArmScale.X = 0.005f; ArmScale.Y = 0.005f; ArmScale.Z = 0.005f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);

		RelativeLeftArmLocation.X = -0.136914f; RelativeLeftArmLocation.Y = 0.130597f; RelativeLeftArmLocation.Z = -0.028364f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = 29.988533f; RelativeLeftArmRotation.Yaw = 156.783386f; RelativeLeftArmRotation.Roll = -112.455589f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = -0.141102f; RelativeRightArmLocation.Y = 0.015456f; RelativeRightArmLocation.Z = 0.014103f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = 7.867325f; RelativeRightArmRotation.Yaw = 95.053917f; RelativeRightArmRotation.Roll = 89.999779f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(77.0f, -8.0f, 108.0f); ADSIntermediaryLocation.Set(77.0f, -14.1f, 120.5f);

		gunBaseDamage = 20;
		break;
	case 3:
		//RelativeGunlocation.Set(-9.0f, 80.0f, 125.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 0.956615f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 1.0f; GunScale.Y = 1.0f; GunScale.Z = 1.0f; Gun->SetRelativeScale3D(GunScale);

		MagazineScale.X = 0.0f; MagazineScale.Y = 0.0f; MagazineScale.Z = 0.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);

		AdsRelativeGunLocation.X = 2.20f; AdsRelativeGunLocation.Y = 84.0f; AdsRelativeGunLocation.Z = 125.4f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 0.0f;

		//arms
		ArmScale.X = .025; ArmScale.Y = .025; ArmScale.Z = .025;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);
		RelativeLeftArmLocation.X = 0.258656f; RelativeLeftArmLocation.Y = -0.749805f; RelativeLeftArmLocation.Z = -0.048086f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = -58.591228f; RelativeLeftArmRotation.Yaw = 32.205643f; RelativeLeftArmRotation.Roll = -116.262787f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = -0.284246f; RelativeRightArmLocation.Y = -0.727555f; RelativeRightArmLocation.Z = -0.251357f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = -68.481956f; RelativeRightArmRotation.Yaw = 107.846153f; RelativeRightArmRotation.Roll = 148.189743f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(77.0f, -4.0f, 120.5f); ADSIntermediaryLocation.Set(59.0f, -15.9f, 125.f);

		gunBaseDamage = 10;
		break;

	case 4:
		//RelativeGunlocation.Set(-9.0f, 80.0f, 125.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = 273.599f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 0.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 1.0f; GunScale.Y = 1.0f; GunScale.Z = 1.0f; Gun->SetRelativeScale3D(GunScale);

		MagazineScale.X = 0.0f; MagazineScale.Y = 0.0f; MagazineScale.Z = 0.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);

		AdsRelativeGunLocation.X = 2.20f; AdsRelativeGunLocation.Y = 84.0f; AdsRelativeGunLocation.Z = 125.4f;
		AdsGunRotation.Roll = 273.599f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 0.0f;

		//arms
		ArmScale.X = .025; ArmScale.Y = .025; ArmScale.Z = .025;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);
		RelativeLeftArmLocation.X = 0.258656f; RelativeLeftArmLocation.Y = -0.749805f; RelativeLeftArmLocation.Z = -0.048086f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = -58.591228f; RelativeLeftArmRotation.Yaw = 32.205643f; RelativeLeftArmRotation.Roll = -116.262787f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = -0.284246f; RelativeRightArmLocation.Y = -0.727555f; RelativeRightArmLocation.Z = -0.251357f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = -68.481956f; RelativeRightArmRotation.Yaw = 107.846153f; RelativeRightArmRotation.Roll = 148.189743f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(113.0f, -1.0f, 145.0f); ADSIntermediaryLocation.Set(113.0f, -1.0f, 145.0f);

		gunBaseDamage = 8;
		break;

	case 7:
		//RelativeGunlocation.Set(-115.0f, 77.0f, 133.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 90.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 0.15f; GunScale.Y = 0.15f; GunScale.Z = 0.15f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = 0.0f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 270.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 0.175f; MagazineScale.Y = 0.175f; MagazineScale.Z = 0.175f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 120.4985f; RelativeMagazineLocation.Y = 28.1396f; RelativeMagazineLocation.Z = -19.056f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = -104.7f; AdsRelativeGunLocation.Y = 68.0f; AdsRelativeGunLocation.Z = 145.0f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 90.0f;

		//arms
		ArmScale.X = 1.5f; ArmScale.Y = 1.5f; ArmScale.Z = 1.5f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);
		RelativeLeftArmLocation.X = -19.0f; RelativeLeftArmLocation.Y = -7.0f; RelativeLeftArmLocation.Z = -35.0f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = -11.5f; RelativeLeftArmRotation.Yaw = 72.3f; RelativeLeftArmRotation.Roll = 150.0f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = 7.7f; RelativeRightArmLocation.Y = 41.2f; RelativeRightArmLocation.Z = -16.5f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = -76.0f; RelativeRightArmRotation.Yaw = 60.8f; RelativeRightArmRotation.Roll = 142.1f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(88.0f, 117.0f, 128.0f); ADSIntermediaryLocation.Set(60.0f, 90.5f, 145.0f);

		gunBaseDamage = 17;
		break;

	case 8: break;

	case 13:
		//RelativeGunlocation.Set(-6.0f, 79.0f, 145.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 270.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 0.15f; GunScale.Y = 0.15f; GunScale.Z = 0.15f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = 0.0f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 270.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 0.0f; MagazineScale.Y = 0.0f; MagazineScale.Z = 0.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 120.4985f; RelativeMagazineLocation.Y = 28.1396f; RelativeMagazineLocation.Z = -19.056f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = .170652f; AdsRelativeGunLocation.Y = 53.9986f; AdsRelativeGunLocation.Z = 153.8394f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.1f; AdsGunRotation.Yaw = 269.999f;

		//arms
		ArmScale.X = 0.3f; ArmScale.Y = 0.3f; ArmScale.Z = 0.3f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);
		RelativeLeftArmLocation.X = -2.9f; RelativeLeftArmLocation.Y = 2.1f; RelativeLeftArmLocation.Z = -7.1f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Roll = 161.832001f; RelativeLeftArmRotation.Pitch = -1.19371f; RelativeLeftArmRotation.Yaw = -22.642088f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = 10.4f; RelativeRightArmLocation.Y = -0.5f; RelativeRightArmLocation.Z = -0.7f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = -84.358635f; RelativeRightArmRotation.Yaw = -179.999969f; RelativeRightArmRotation.Roll = -179.999634f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(76.0f, 0.0f, 140.0f); ADSIntermediaryLocation.Set(51.0f,-14.5f,154.2f);

		gunBaseDamage = 17;
		break;

	case 20: 
		GunRotation.Roll = 230.0f; GunRotation.Pitch = 180.0f; GunRotation.Yaw = 0.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 0.1f; GunScale.Y = 0.1f; GunScale.Z = 0.1f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = -90.f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 0.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 1.0f; MagazineScale.Y = 1.0f; MagazineScale.Z = 1.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 0.0f; RelativeMagazineLocation.Y = 0.152f; RelativeMagazineLocation.Z = 0.0f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = 0.03f; AdsRelativeGunLocation.Y = 103.0f; AdsRelativeGunLocation.Z = 120.5f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 89.999f;

		//arms
		ArmScale.X = 0.005f; ArmScale.Y = 0.005f; ArmScale.Z = 0.005f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);

		RelativeLeftArmLocation.X = -0.136914f; RelativeLeftArmLocation.Y = 0.130597f; RelativeLeftArmLocation.Z = -0.028364f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = 29.988533f; RelativeLeftArmRotation.Yaw = 156.783386f; RelativeLeftArmRotation.Roll = -112.455589f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = -0.141102f; RelativeRightArmLocation.Y = 0.015456f; RelativeRightArmLocation.Z = 0.014103f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = 7.867325f; RelativeRightArmRotation.Yaw = 95.053917f; RelativeRightArmRotation.Roll = 89.999779f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(45.0f, 8.0f, 145.029f); ADSIntermediaryLocation.Set(77.0f, -14.1f, 120.5f);

		gunBaseDamage = 35;
		break;

	case 24:
		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 0.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 0.4f; GunScale.Y = 0.4f; GunScale.Z = 0.4f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = -90.f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 0.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 1.0f; MagazineScale.Y = 1.0f; MagazineScale.Z = 1.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 0.0f; RelativeMagazineLocation.Y = 0.152f; RelativeMagazineLocation.Z = 0.0f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = -14.11f; AdsRelativeGunLocation.Y = 0.0f; AdsRelativeGunLocation.Z = 0.0f;
		AdsGunRotation.Roll = 352.8f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 7.2f;

		//arms
		ArmScale.X = 3.0f; ArmScale.Y = 3.0f; ArmScale.Z = 3.0f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);

		RelativeLeftArmLocation.X = 27.0f; RelativeLeftArmLocation.Y = -117.0f; RelativeLeftArmLocation.Z = 30.0f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = 273.0f; RelativeLeftArmRotation.Yaw = 93.58f; RelativeLeftArmRotation.Roll = 187.0f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = -21.0f; RelativeRightArmLocation.Y = -112.94f; RelativeRightArmLocation.Z = 30.0f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = 266.39f; RelativeRightArmRotation.Yaw = 279.39f; RelativeRightArmRotation.Roll = 341.3f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(71.0f,-3.0f,134.0f); ADSIntermediaryLocation.Set(49.0f, -3.0f, 145.0f);

		gunBaseDamage = 30;
		break;

	case 25:
		//RelativeGunlocation.Set(-3.0f, 43.0f, 150.0f); Gun->SetRelativeLocation(RelativeGunlocation);
		GunRotation.Roll = -0.28312f; GunRotation.Pitch = -0.0022f; GunRotation.Yaw = 0.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 1.0f; GunScale.Y = 1.0f; GunScale.Z = 1.0f; Gun->SetRelativeScale3D(GunScale);

		MagazineScale.X = 0.0f; MagazineScale.Y = 0.0f; MagazineScale.Z = 0.0f; MeshMagazine->SetRelativeScale3D(MagazineScale);

		AdsRelativeGunLocation.X = 0.076412f; AdsRelativeGunLocation.Y = 43.0f; AdsRelativeGunLocation.Z = 154.0f;
		AdsGunRotation.Roll = -0.000001f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 90.000008f;

		//arms
		ArmScale.X = .02f; ArmScale.Y = .02f; ArmScale.Z = .02f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);
		RelativeLeftArmLocation.X = 0.186158f; RelativeLeftArmLocation.Y = -0.48665f; RelativeLeftArmLocation.Z = -0.01687f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = -86.3908f; RelativeLeftArmRotation.Yaw = -0.02571f; RelativeLeftArmRotation.Roll = -61.167f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = -0.14423f; RelativeRightArmLocation.Y = -0.51406f; RelativeRightArmLocation.Z = -0.11239f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = -75.5992f; RelativeRightArmRotation.Yaw = 151.1990f; RelativeRightArmRotation.Roll = 111.5992f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(49.0f, -7.0f, 149.3649f); ADSIntermediaryLocation.Set(68.0f, -14.0f, 145.0f);

		gunBaseDamage = 45;
		break;
	case 26:
		//RelativeGunlocation.Set(-14.0f, 85.0f, 116.0f); Gun->SetRelativeLocation(RelativeGunlocation);

		GunRotation.Roll = 0.0f; GunRotation.Pitch = 0.0f; GunRotation.Yaw = 0.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 1.8f; GunScale.Y = 1.8f; GunScale.Z = 1.8f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = 270.f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 0.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 0.01f; MagazineScale.Y = 0.01f; MagazineScale.Z = 0.01f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 0.0f; RelativeMagazineLocation.Y = 0.152f; RelativeMagazineLocation.Z = 0.0f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = -14.2045f; AdsRelativeGunLocation.Y = 0.0f; AdsRelativeGunLocation.Z = 0.0f;
		AdsGunRotation.Roll = 0.0f; AdsGunRotation.Pitch = 0.0f; AdsGunRotation.Yaw = 0.0f;

		//arms
		ArmScale.X = 0.9f; ArmScale.Y = 0.9f; ArmScale.Z = 0.9f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);

		RelativeLeftArmLocation.X = 10.82252f; RelativeLeftArmLocation.Y = 11.98381f; RelativeLeftArmLocation.Z = -12.0621f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = -20.5727f; RelativeLeftArmRotation.Yaw = 198.751f; RelativeLeftArmRotation.Roll = -238.982f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = -3.4584f; RelativeRightArmLocation.Y = -34.7869f; RelativeRightArmLocation.Z = 11.11032f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = -78.5222f; RelativeRightArmRotation.Yaw = 164.8864f; RelativeRightArmRotation.Roll = 103.4948f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(76.99831f, 4.0f, 107.999f); ADSIntermediaryLocation.Set(80.19999f, -14.0f, 117.84999f);

		gunBaseDamage = 23;
		break;

	case 28:
		//RelativeGunlocation.Set(-14.0f, 85.0f, 116.0f); Gun->SetRelativeLocation(RelativeGunlocation);

		GunRotation.Roll = 7.2f; GunRotation.Pitch = -1.0f; GunRotation.Yaw = 180.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 2.0f; GunScale.Y = 2.0f; GunScale.Z = 2.0f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = 270.f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 0.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 0.01f; MagazineScale.Y = 0.01f; MagazineScale.Z = 0.01f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 0.0f; RelativeMagazineLocation.Y = 0.152f; RelativeMagazineLocation.Z = 0.0f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = -12.82f; AdsRelativeGunLocation.Y = 0.0f; AdsRelativeGunLocation.Z = 0.0f;
		AdsGunRotation.Roll = 7.2f; AdsGunRotation.Pitch = -1.0f; AdsGunRotation.Yaw = 180.0f;

		//arms
		ArmScale.X = 0.8f; ArmScale.Y = 0.8f; ArmScale.Z = 0.8f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);

		RelativeLeftArmLocation.X = -7.82f; RelativeLeftArmLocation.Y = 3.654f; RelativeLeftArmLocation.Z = 6.3634f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = -9.52532f; RelativeLeftArmRotation.Yaw = 104.268f; RelativeLeftArmRotation.Roll = 12.13496f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = 7.374845f; RelativeRightArmLocation.Y = -0.21207f; RelativeRightArmLocation.Z = 27.11080f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = -2.06621f; RelativeRightArmRotation.Yaw = 79.670f; RelativeRightArmRotation.Roll = -12.8216f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(111.0f, 6.0f, 127.0f); ADSIntermediaryLocation.Set(77.0f, 2.0f, 149.0f);

		gunBaseDamage = 16;
		break;

	case 30:
		//RelativeGunlocation.Set(-14.0f, 85.0f, 116.0f); Gun->SetRelativeLocation(RelativeGunlocation);

		GunRotation.Roll = 7.2f; GunRotation.Pitch = -1.0f; GunRotation.Yaw = 180.0f; Gun->SetRelativeRotation(GunRotation);
		GunScale.X = 2.0f; GunScale.Y = 2.0f; GunScale.Z = 2.0f; Gun->SetRelativeScale3D(GunScale);

		MagazineRotation.Roll = 270.f; MagazineRotation.Pitch = 0.0f; MagazineRotation.Yaw = 0.0f; MeshMagazine->SetRelativeRotation(MagazineRotation);
		MagazineScale.X = 0.01f; MagazineScale.Y = 0.01f; MagazineScale.Z = 0.01f; MeshMagazine->SetRelativeScale3D(MagazineScale);
		RelativeMagazineLocation.X = 0.0f; RelativeMagazineLocation.Y = 0.152f; RelativeMagazineLocation.Z = 0.0f; MeshMagazine->SetRelativeLocation(RelativeMagazineLocation);

		AdsRelativeGunLocation.X = -12.82f; AdsRelativeGunLocation.Y = 0.0f; AdsRelativeGunLocation.Z = 0.0f;
		AdsGunRotation.Roll = 7.2f; AdsGunRotation.Pitch = -1.0f; AdsGunRotation.Yaw = 180.0f;

		//arms
		ArmScale.X = 0.8f; ArmScale.Y = 0.8f; ArmScale.Z = 0.8f;
		LeftArm->SetRelativeScale3D(ArmScale); RightArm->SetRelativeScale3D(ArmScale);

		RelativeLeftArmLocation.X = -7.82f; RelativeLeftArmLocation.Y = 3.654f; RelativeLeftArmLocation.Z = 6.3634f; LeftArm->SetRelativeLocation(RelativeLeftArmLocation);
		RelativeLeftArmRotation.Pitch = -9.52532f; RelativeLeftArmRotation.Yaw = 104.268f; RelativeLeftArmRotation.Roll = 12.13496f; LeftArm->SetRelativeRotation(RelativeLeftArmRotation);
		RelativeRightArmLocation.X = 7.374845f; RelativeRightArmLocation.Y = -0.21207f; RelativeRightArmLocation.Z = 27.11080f; RightArm->SetRelativeLocation(RelativeRightArmLocation);
		RelativeRightArmRotation.Pitch = -2.06621f; RelativeRightArmRotation.Yaw = 79.670f; RelativeRightArmRotation.Roll = -12.8216f; RightArm->SetRelativeRotation(RelativeRightArmRotation);

		IntermediaryLocation.Set(111.0f, 6.0f, 127.0f); ADSIntermediaryLocation.Set(77.0f, 2.0f, 149.0f);

		gunBaseDamage = 0;
		break;

	}

	Intermediary->SetRelativeLocation(IntermediaryLocation);
	//GenerateWeaponText();
}

void AMyCharacter::FireWeaponOrTool()
{
FHitResult* HitResult = new FHitResult();
FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();


//bullet spread
float ranX = FMath::RandRange(-50, 50);
float ranY = FMath::RandRange(-50, 50);
float ranZ = FMath::RandRange(-50, 50);
FVector spreadAdjust = FVector(ranX, ranY, ranZ);

FVector EndTrace = ((forwardVector*20000.f) + StartTrace + spreadAdjust);

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

WeaponRecoil();

//spawn gun flash
FRotator xx; FVector zz = FP_MuzzleLocation->GetComponentLocation(); FActorSpawnParameters SpawnParams;
AActor* GunFlashSpawned = GetWorld()->SpawnActor<AActor>(GunFlash, zz, xx, SpawnParams);
AActor* BulletCasingSpawned = GetWorld()->SpawnActor<AActor>(smallBulletCasing, Gun->GetSocketLocation(TEXT("case")), xx, SpawnParams);
//AActor* BulletCasingSpawned = GetWorld()->SpawnActor<AActor>(smallBulletCasing, zz, xx, SpawnParams);


if (Gun0FireSound != NULL)
{
	UGameplayStatics::PlaySoundAtLocation(this, Gun0FireSound, GetActorLocation());
}

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
			HitmarkerOn();
			UGameplayStatics::PlaySoundAtLocation(this, hitsound, GetActorLocation());
			GetWorld()->GetTimerManager().SetTimer(hitmarkertimer, this, &AMyCharacter::HitmarkerOff, 0.1f, false);
			FRotator PlaceHolder; FActorSpawnParameters SpawnParams; AActor* SpawnBloodEffect = GetWorld()->SpawnActor<AActor>(BloodEffect, HitResult->ImpactPoint, PlaceHolder, SpawnParams);
		}

		FVector_NetQuantizeNormal Locs;
		FRotator Rots;
		Locs = HitResult->Location;
		SpawnBulletImpact(Locs, Rots);


	}
}

Inventory->AmmoInWeapon[InventoryIndexEquipped] -= 1;

}

void AMyCharacter::SpawnBulletImpact(FVector Loc, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(BulletImpact, Loc, Rot, SpawnParams);
}
void AMyCharacter::MeleeWeaponFire()
{
	if (meleeTimer < 1) {
		FireWeaponOrTool();
		//after fire
		MeleeHandler();
	}
}
void AMyCharacter::MeleeHandler()
{
	meleeTimer = rateOfFire;
	DecreaseMeleeTimer();
}
void AMyCharacter::DecreaseMeleeTimer()
{
	meleeTimer -= 1;
	if (meleeTimer > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(MeleePin, this, &AMyCharacter::DecreaseMeleeTimer, 2.0f, false);
    }
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

		if (ammoInMagazine > 0) {
			FireWeaponOrTool();
			//FireSoundAndAnimation();
			SpawnGunSmoke();
			ammoInMagazine -= 1;
			GenerateWeaponText();
			GetWorld()->GetTimerManager().SetTimer(firingPin, this, &AMyCharacter::FullyAutomaticFire, rateOfFire, false);
		}
			
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
	if (Paused == false) 
	{
		isFiring = true;
		//check for ammo and gun type to determine rate of fire
		switch (currentFireType)
		{
		case 0: if (ammoInMagazine > 0) { SemiAutomaticFire(); } break;
		case 1: if (ammoInMagazine > 0) { FullyAutomaticFire(); } break;
		case 2: GatherWater(); break;
		case 3: ammoInMagazine = 1; MeleeWeaponFire();
		case 4: ammoInMagazine = 1; MeleeWeaponFire(); TryMakeDirt();  break;
		case 5: if (ammoInMagazine > 0) { TryPlantSeed();  }  break;
		}
	}
}

void AMyCharacter::TryPlantSeed() {
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*20000.f) + StartTrace);

	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams)) {

		APlantObject* TestTarget = Cast<APlantObject>(HitResult->Actor.Get());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));

			if (TestTarget != NULL && !TestTarget->IsPendingKill()) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Test Target not null")));
				if (TestTarget->numseedplanted == 0) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("seednum is 0")));
					TestTarget->PlantSeed(PlantSeedSelected);
					//FireWeaponOrTool();
				}
			}
	}
}

void AMyCharacter::TryMakeDirt() {
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*20000.f) + StartTrace);
	HEALTH = 1; STAMINA = 1; HUNGER = 1; WATER = 1; FATIGUE = 1;
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	if (STAMINA > 0.05f) {
		if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams)) {

			if (FVector::Distance(HitResult->Location, GetActorLocation()) < 100.0f) {
				//make dirt
				farmClass->AddDirt(HitResult->Location);
			}

		}
		STAMINA -= .01f;
		FATIGUE -= .01f;
		WATER -= .01f;
		HUNGER -= .01f;
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

void AMyCharacter::ReleaseTrigger() {
	isFiring = false; AddControllerPitchInput(.2f * pitchReset); pitchReset = 0;
}

void AMyCharacter::GenerateWeaponText()
{
	FString Caliber = Inventory->GetWeaponCaliber(CurrentEquippedWeapon);
	int availableAmmunition = 0;
	if (CurrentEquippedWeapon != 6666) {
		availableAmmunition = Inventory->GetAmmoAvailableForGivenWeapon(CurrentEquippedWeapon);
	}
	FString availammo = FString::FromInt(availableAmmunition);
	WeaponText = FString::FromInt(ammoInMagazine) + "/" + availammo + " " + "[ " + Caliber + " ]";
}

//Initially created for the "water gathering" mechanic, this is used to store where in the inventory the player's current tool or weapon resides.
//Why? This way, when something like water gathering occurs, we can set the arrays in the inventory refering to item contents and amounts at the appropriate index
//EG: the inventory may hold value [8] in a slot 7 of the item ID array. as 8 refer to plastic containers, when we gather water, we can set slot 7 of the contents array to "unboiled water."
void AMyCharacter::ChangeItemIndexEquipped(int IndexNumber)
{
	InventoryIndexEquipped = IndexNumber;
}

//take damage
void AMyCharacter::DamageTarget(int damage)
{
	HEALTH -= damage;
}


void AMyCharacter::WeaponRecoil()
{
	FVector Loc = Gun->GetRelativeLocation();
	Loc.Z += 1.0f;
	Gun->SetRelativeLocation(Loc);
	AddControllerPitchInput(-.2f);
	pitchReset += 1;
}


bool AMyCharacter::CheckLoaderChecker()
{
	bool retval = false;

	FString Location = "/tmp/ToggleMainMenuAndActiveCharacterBP.txt"; FString Pathf = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + Location;
	TArray<FString> LoadedFile;
	//Load the file into an array, each line is saved as an array element. These lines are Comma Separated Values
	FFileHelper::LoadANSITextFileToStrings(*Pathf, NULL, LoadedFile);
	if (LoadedFile[0] == "true")
	{
		retval = true;
	}


	return retval;
}


void AMyCharacter::SetLoaderChecker()
{
	FString Location = "/tmp/ToggleMainMenuAndActiveCharacterBP.txt"; FString Pathf = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + Location;
	FString savee = "false";
	FFileHelper::SaveStringToFile(savee, *Pathf, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
}
