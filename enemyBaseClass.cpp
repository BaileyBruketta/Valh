// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyBaseClass.h"
#include "enemyHandler.h"
#include "Engine.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"

// Sets default values
AenemyBaseClass::AenemyBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	capsule->SetupAttachment(RootComponent);

	MeshBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	MeshBody->bCastDynamicShadow = true;
	MeshBody->CastShadow = true;
	MeshBody->SetupAttachment(capsule);
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
	EnemyPushBack();
}

void AenemyBaseClass::SetGlobalID(int ID)
{
	globalIdentifier = ID;
}

void AenemyBaseClass::EnemyPushBack()
{
	FVector location;
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FRotator weSmokeOpps;
	FVector whereHeAt;

	whereHeAt = player->GetActorLocation();

	weSmokeOpps = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), whereHeAt);

	this->SetActorRotation(weSmokeOpps);

	FVector whereImAt;
	whereImAt = this->GetActorLocation();
	whereImAt += this->GetActorForwardVector() * -30.0f;

	float dist = FVector::Dist(whereImAt, whereHeAt);

	this->SetActorRelativeLocation(whereImAt);

}

void AenemyBaseClass::DeathDrop()
{

}
