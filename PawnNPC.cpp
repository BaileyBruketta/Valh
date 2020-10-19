// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnNPC.h"
#include "enemyHandler.h"
#include "Engine.h"

// Sets default values
APawnNPC::APawnNPC()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;
	MeshBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	MeshBody->bCastDynamicShadow = true;
	MeshBody->CastShadow = true;
	MeshBody->SetupAttachment(CapsuleComponent);
	playerCharacterReference = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("mover"));

}

// Called when the game starts or when spawned
void APawnNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APawnNPC::ChangeMesh(USkeletalMesh* MeshIn)
{
	MeshBody->SetSkeletalMesh(MeshIn);
}

void APawnNPC::SABP(UAnimBlueprint* animbp)
{
	MeshBody->SetAnimClass(animbp->GetAnimBlueprintGeneratedClass());
}

void APawnNPC::OnSpawn(AenemyHandler* theOne)
{
	theHandler = theOne;
	AenemyHandler* kit = theOne;
	theHandler = kit;
}

//this is set at spawn and used to identify the unit
void APawnNPC::SetGlobalID(int ID)
{
	globalIdentifier = ID;
}

