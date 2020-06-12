// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyBaseClass.h"
#include "enemyHandler.h"
#include "Engine.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "MyCharacter.h"

// Sets default values
AenemyBaseClass::AenemyBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	//capsule->SetupAttachment(RootComponent);
	//SetRootComponent(capsule);
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(ACharacter::CapsuleComponentName);
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	//bodyDummy = CreateDefaultSubobject<UStaticMesh>(TEXT("BodyDummy"));
	//bodyDummy->bCastDynamicShadow = false;
	//bodyDummy->CastShadow = false;
	//SetRootComponent(bodyDummy);

	MeshBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	MeshBody->bCastDynamicShadow = true;
	MeshBody->CastShadow = true;
	MeshBody->SetupAttachment(CapsuleComponent);
	//SetRootComponent(MeshBody);

	primaryNextLocation.Set(0.0f, 0.0f, 0.0f);

	State = 1;
	StateTimer = 10;
	Speed = 20;
	playerCharacterReference = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called when the game starts or when spawned
void AenemyBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AenemyBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AenemyBaseClass::OnSpawn(AenemyHandler* theOne)
{
	theHandler = theOne;
	AenemyHandler* kit = theOne;
	theHandler = kit;
	
}


void AenemyBaseClass::DamageTarget(float Damage)
{

	theHandler->DecreaseHealth(globalIdentifier, Damage);
	//EnemyPushBack();

	//This should run a check against "enemyType," but this object does not currently hold that information
	RunFromPlayer();
}

//this is set at spawn and used to identify the unit
void AenemyBaseClass::SetGlobalID(int ID)
{
	globalIdentifier = ID;
}

//this is no longer in use 
void AenemyBaseClass::EnemyPushBack()
{
	FVector location;
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FRotator newRot;
	FVector otherLoc;

	otherLoc = player->GetActorLocation();

	newRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), otherLoc);

	this->SetActorRotation(newRot);

	FVector whereImAt;
	whereImAt = this->GetActorLocation();
	whereImAt += this->GetActorForwardVector() * -30.0f;

	float dist = FVector::Dist(whereImAt, otherLoc);
	this->SetActorRelativeLocation(whereImAt);

}

void AenemyBaseClass::DeathDrop(){	//probably not going to use this anymore as this functionality is now handled in the enemyHandler
}

//this is called after a state switch to find the next location to move towards
void AenemyBaseClass::FindNextDestination()
{
	//Find a new location very close to the current location
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation;
	NewLocation.X += (floor(rand() % 3 + 10) - floor(rand() % 3 + 10));
	NewLocation.Y += (floor(rand() % 3 + 10) - floor(rand() % 3 + 10));
	FRotator newRot; newRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NewLocation);
	SetActorRotation(newRot);

}

//the enemyhandler calls this and prompts it to do updates
void AenemyBaseClass::UpdateReceived()
{
	//See if player is "too close for comfort"
	PlayerDistanceCheck();

	//atatch character to the ground
	ClipToGround();

	//evaluate state of unit 
	switch (State)
	{
		//waiting, idle, etc
		case 0 : 	
			break;

		//walking
		case 1 :
			FVector forwardvect = GetActorForwardVector(); forwardvect = forwardvect * Speed;
			FVector Loc = GetActorLocation();
			FVector newvec = Loc + forwardvect;
			SetActorLocation(newvec);
			break;
	}

	//reduce state timer
	StateTimer -= 1;

	//evaluate whether to rotate to a new state
	if (StateTimer < 1)
	{
		NewState();
	}
}


//does a random number to give the unit a new state
void AenemyBaseClass::NewState()
{
	int newState = floor(rand() % 2 );
	Speed = 20;

	switch (newState)
	{
	case 0: State = 0;  StateTimer += 60; break;
	case 1: State = 1;  FindNextDestination(); StateTimer += 10; break;
	}
	
}


//ignores this actor, casts a ray to the ground from the sky, moves actor to be on the ground/not floating as actor does not use physics gravity
void AenemyBaseClass::ClipToGround()
{
	FHitResult* HitResult = new FHitResult(); FVector StartTrace = GetActorLocation(); StartTrace.Z += 5000.0f;
	FVector EndTrace = GetActorLocation(); EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams(); TraceParams->AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		FVector NewLoc = StartTrace;
		NewLoc.Z = HitResult->Location.Z; NewLoc.Z += 75.0f;
		SetActorLocation(NewLoc);

	}
}

void AenemyBaseClass::RandomRotation()
{

}

//check to see if the player is within range
void AenemyBaseClass::PlayerDistanceCheck()
{
	//calculate distance between player and this unit
	FVector ThisLocation   = GetActorLocation();
	FVector PlayerLocation = playerCharacterReference->GetActorLocation();
		//UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	float   distance       = FVector::Dist(ThisLocation, PlayerLocation);

	if (distance < 1000)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("An enemy has been alerted to your presence")));
		RunFromPlayer();
	}
	
}

//change direction to face away from the player
void AenemyBaseClass::RunFromPlayer()
{
	Speed = 35;  //increase speed when running away, this is lowered when the run phase ends
	State = 1; StateTimer = 60;

	FVector  PlayerLocation = playerCharacterReference->GetActorLocation();
		//UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector  ThisLocation   = GetActorLocation();
	FRotator newRot; newRot = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, ThisLocation);
	SetActorRotation(newRot);
}