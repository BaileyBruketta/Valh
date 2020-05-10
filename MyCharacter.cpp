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

	//Create Arms
	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->bCastDynamicShadow = true;
	Arms->CastShadow = true;
	Arms->SetupAttachment(FirstPersonCameraComponent);

	//Gun 
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunWithArms"));
	Gun->bCastDynamicShadow = true;
	Gun->CastShadow = true;
	Gun->SetupAttachment(Arms);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	RelativeGunlocation    = new FVector;
	AdsRelativeGunLocation = new FVector;
	GunRotation            = new FRotator;
	AdsGunRotation         = new FRotator;
	GunScale               = new FVector;
	gunBaseDamage          = 0;
	ammoInMagazine         = 1000;
	currentFireType        = 0;
	isFiring               = false;
	GunOffset              = FVector(100.0f, 0.0f, 10.0f);



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

	//Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	

}

//Game World Interactions
void AMyCharacter::Interact()
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*200.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		AItemPickup* TestTarget = Cast<AItemPickup>(HitResult->Actor.Get());

		if (TestTarget != NULL && !TestTarget->IsPendingKill())
		{
			//pick up item
			int ItemID = TestTarget->PickUpItem();

			//if (ItemID != NULL) 
			//{
				
				Inventory->AddToInventory(ItemID);

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
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

//Guns

void AMyCharacter::ChangeGunEquipped(int gunNumber)
{
	//set the mesh
	switch (gunNumber) 
	{
	case 0: Gun->SetSkeletalMesh(Gun0MeshReference);
		break;
	}
}

void AMyCharacter::SetGunVariables(int gunNumber)
{
	//set location, adslocation, rotation, ads rotation, scale, gunbasedamage, rateoffire

	//switch (gunNumber)
	//{
	//case 0: 
	//}
}

void AMyCharacter::FireWeaponOrTool()
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

}
void AMyCharacter::FullyAutomaticFire()
{
	firearmTimer -= 1;
	if (firearmTimer < 5)
	{
		if (ammoInMagazine > 0)
		{
			FireWeaponOrTool();
			//FireSoundAndAnimation();
			SpawnGunSmoke();
			ammoInMagazine -= 1;

			firearmTimer = rateOfFire;
		}
	}
}

void AMyCharacter::SpawnGunSmoke()
{
	FRotator SpawnRotation;
	SpawnRotation = GetControlRotation();

	FVector x;
	x = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);


	FActorSpawnParameters SpawnParams;
	AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(GunSmoke, x, SpawnRotation, SpawnParams);
	FVector Scale = FVector(.2f, .2f, .2f);
	SPawnedActorRef->SetActorScale3D(Scale);
}

void AMyCharacter::OnFire()
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

