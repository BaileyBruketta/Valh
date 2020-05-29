// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageGod.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Engine.h"


// Sets default values
AFoliageGod::AFoliageGod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

// Called when the game starts or when spawned
void AFoliageGod::BeginPlay()
{
	Super::BeginPlay();
	SpawnOne();

	
}

// Called every frame
void AFoliageGod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//TODO: rewrite as an iteration using arrays for plant object/ number of plants per plant type
void AFoliageGod::SpawnOne()
{
	FVector Locs = GetActorLocation();
	FRotator Rots = GetActorRotation();
	FActorSpawnParameters SpawnParams;


	//shortgrass
	int numberOfPlants = floor(rand() % 800+1000);

	for (int i = 0; i < numberOfPlants; i++)
	{
		bool underwater = true;
		while (underwater == true) 
		{
			//new location
			float newx = FMath::FRandRange(-50000,50000); Locs.X = newx;
			float newy = FMath::FRandRange(-50000,50000); Locs.Y = newy;

			//test height
			FVector HeightTest = Locs; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				Locs.Z = HitResult->Location.Z;
			}

			if (Locs.Z > 471) { underwater = false; }


		}


		//spawn plant
		AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(plant0, Locs, Rots, SpawnParams);

		//resize plant
		FVector Scale = Locs; float f = rand() % 5; Scale.X = f; Scale.Y = f; Scale.Z = f;

		//set plant scale
		SPawnedActorRef->SetActorScale3D(Scale);

	}
	
	//trees
	numberOfPlants = floor(rand() % 3+ 50);

	for (int i = 0; i < numberOfPlants; i++)
	{
		bool underwater = true;
		while (underwater == true)
		{

			float newx = (rand() % 50000) + (rand() % 50000 * -1); Locs.X = newx;
			float newy = (rand() % 50000) + (rand() % 50000 * -1); Locs.Y = newy;

			//test height
			FVector HeightTest = Locs; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				Locs.Z = HitResult->Location.Z;
			}

			if (Locs.Z > 471) { underwater = false; }

		}



		AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(plant1, Locs, Rots, SpawnParams);

		FVector Scale = Locs; float f = rand() % 9; Scale.X = f; Scale.Y = f; Scale.Z = f;

		SPawnedActorRef->SetActorScale3D(Scale);

	}

	//purple grass
	numberOfPlants = floor(rand() % 300+2000);

	for (int i = 0; i < numberOfPlants; i++)
	{
		bool underwater = true;
		while (underwater == true)
		{

			float newx = (rand() % 50000) + (rand() % 50000 * -1); Locs.X = newx;
			float newy = (rand() % 50000) + (rand() % 50000 * -1); Locs.Y = newy;

			//test height
			FVector HeightTest = Locs; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				Locs.Z = HitResult->Location.Z;
			}
			if (Locs.Z > 471) { underwater = false; }

		}
		AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(plant2, Locs, Rots, SpawnParams);

		FVector Scale = Locs; float f = rand() % 3; Scale.X = f; Scale.Y = f; Scale.Z = f;

		SPawnedActorRef->SetActorScale3D(Scale);

	}


	//wheatgrass
	numberOfPlants = floor(rand() % 100+600);

	for (int i = 0; i < numberOfPlants; i++)
	{
		bool underwater = true;
		while (underwater == true)
		{

			float newx = (rand() % 50000) + (rand() % 50000 * -1); Locs.X = newx;
			float newy = (rand() % 50000) + (rand() % 50000 * -1); Locs.Y = newy;

			//test height
			FVector HeightTest = Locs; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				Locs.Z = HitResult->Location.Z;
			}

			if (Locs.Z > 471) { underwater = false; }
		}

		AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(plant3, Locs, Rots, SpawnParams);

		FVector Scale = Locs; float f = rand() % 3; Scale.X = f; Scale.Y = f; Scale.Z = f;

		SPawnedActorRef->SetActorScale3D(Scale);

	}

	//green grass
	numberOfPlants = floor(rand() % 100 + 600);

	for (int i = 0; i < numberOfPlants; i++)
	{
		bool underwater = true;
		while (underwater == true)
		{

			float newx = (rand() % 50000) + (rand() % 50000 * -1); Locs.X = newx;
			float newy = (rand() % 50000) + (rand() % 50000 * -1); Locs.Y = newy;

			//test height
			FVector HeightTest = Locs; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				Locs.Z = HitResult->Location.Z;
			}

			if (Locs.Z > 471) { underwater = false; }
		}

		AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(plant4, Locs, Rots, SpawnParams);

		FVector Scale = Locs; float f = rand() % 3; Scale.X = f; Scale.Y = f; Scale.Z = f;

		SPawnedActorRef->SetActorScale3D(Scale);

	}

	//other trees
	numberOfPlants = floor(rand() % 3 + 50);

	for (int i = 0; i < numberOfPlants; i++)
	{
		bool underwater = true;
		while (underwater == true)
		{

			float newx = (rand() % 50000) + (rand() % 50000 * -1); Locs.X = newx;
			float newy = (rand() % 50000) + (rand() % 50000 * -1); Locs.Y = newy;

			//test height
			FVector HeightTest = Locs; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				Locs.Z = HitResult->Location.Z;
			}

			if (Locs.Z > 471) { underwater = false; }
		}

		AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(plant5, Locs, Rots, SpawnParams);

		FVector Scale = Locs; float f = rand() % 3; Scale.X = f; Scale.Y = f; Scale.Z = f;

		SPawnedActorRef->SetActorScale3D(Scale);

	}

	//other trees
	numberOfPlants = floor(rand() % 1500 + 4000);

	for (int i = 0; i < numberOfPlants; i++)
	{
		bool underwater = true;
		while (underwater == true)
		{

			float newx = (rand() % 50000) + (rand() % 50000 * -1); Locs.X = newx;
			float newy = (rand() % 50000) + (rand() % 50000 * -1); Locs.Y = newy;

			//test height
			FVector HeightTest = Locs; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				Locs.Z = HitResult->Location.Z;
			}

			if (Locs.Z > 471) { underwater = false; }
		}

		AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(plant6, Locs, Rots, SpawnParams);

		FVector Scale = Locs; float f = rand() % 3; Scale.X = f; Scale.Y = f; Scale.Z = f;

		SPawnedActorRef->SetActorScale3D(Scale);

	}

	//spruce tree
	numberOfPlants = floor(rand() % 5 + 50);

	for (int i = 0; i < numberOfPlants; i++)
	{
		bool underwater = true;
		while (underwater == true)
		{

			float newx = (rand() % 50000) + (rand() % 50000 * -1); Locs.X = newx;
			float newy = (rand() % 50000) + (rand() % 50000 * -1); Locs.Y = newy;

			//test height
			FVector HeightTest = Locs; HeightTest.Z = 8000; FHitResult* HitResult = new FHitResult(); FVector StartTrace = HeightTest;
			FVector EndTrace = HeightTest; EndTrace.Z = -800; FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				Locs.Z = HitResult->Location.Z;
			}

			if (Locs.Z > 471) { underwater = false; }
		}

		AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(plant7, Locs, Rots, SpawnParams);

		FVector Scale = Locs; float f = rand() % 3; Scale.X = f; Scale.Y = f; Scale.Z = f;

		SPawnedActorRef->SetActorScale3D(Scale);

	}
}




