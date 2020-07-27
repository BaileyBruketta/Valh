// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycle.h"
#include "Engine.h"
#include "MyCharacter.h"


// Sets default values
ADayNightCycle::ADayNightCycle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	hour = 9;
	min = 0;
	//hour = 3;
	//min = 55;
}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay()
{
	Super::BeginPlay();
	UpdateTime();
	MyCharacterRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	
}

// Called every frame
void ADayNightCycle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADayNightCycle::UpdateTime()
{	//1.0->.25->.0625 then, i cut the amount of minutes increase in half while keeping the timer set to .0625. .0625 is good and smooth for refresh, but too fast for the actual time of day
	min += .015625f;
	if (min > 59) { hour += 1; ResetMinutes(); }
	if (hour >= 25) { hour = 1; }

	UpdateIntensity();
	AMyCharacter* MyChar = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MyChar->SetHoursMinutes(hour, min);

	//initally every 1.0f; turned min to dloat and set to .25f for smoothness
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADayNightCycle::UpdateTime, .0625f, false);
}

void ADayNightCycle::ResetMinutes()
{
	min = 0;
}

void ADayNightCycle::UpdateIntensity()
{
	if (Sun != NULL) 
	{
		FRotator Rotat; Rotat.Pitch = 0.0f; Rotat.Yaw = 180.0f; Rotat.Roll = 0.0f;

		if (hour < 4) { Sun->SetIntensity(0.3f); 
		                                 Rotat.Pitch = 99; SetActorRotation(Rotat);
		} 
		if (hour >= 4) { if (hour < 5) { Sun->SetIntensity(0.3f + (1.7f * (min / 60.0f))); 
		Rotat.Pitch = (100.0f + (82.0f * (min / 60.0f))); SetActorRotation(Rotat);
		} }
		if (hour >= 5) { if (hour < 8) { Sun->SetIntensity(2.0f + (2.0f * ((hour - 5.0f) / 3.0f)) + (2.0f * ((min / 60.0f) / 3.0f))); 
		Rotat.Pitch = (182.0f + (40.0f * ((hour - 5.0f) / 3.0f)) + (40.0f * ((min / 60.0f) / 3.0f))); SetActorRotation(Rotat);
		} }
		if (hour >= 8) { if (hour < 12) { Sun->SetIntensity(4.0f + (1.0f * ((hour - 8.0f) / 4.0f)) + (2.0f * ((min / 60.0f) / 4.0f))); 
		Rotat.Pitch = (222.0f + (40.0f * ((hour - 8.0f) / 4.0f)) + (40.0f * ((min / 60.0f) / 4.0f))); SetActorRotation(Rotat);
		}}
		if (hour >= 12) { if (hour < 16) { Sun->SetIntensity(5.0f - (2.0f * ((hour - 12.0f) / 4.0f)) - (2.0f * ((min / 60.0f) / 4.0f))); 
		Rotat.Pitch = (266.0f + (35.0f * ((hour -12.0f) / 4.0f)) - (35.0f * ((min / 60.0f) / 4.0f))); SetActorRotation(Rotat);
		} }
		if (hour >= 16) { if (hour < 19) { Sun->SetIntensity(4.0f - (2.0f * ((hour - 16.0f) / 3.0f)) - (2.0f * ((min / 60.0f) / 3.0f))); 
		Rotat.Pitch = (297.0f + (54.0f * ((hour - 16.0f) / 3.0f)) - (2.0f * ((min / 60.0f) / 3.0f))); SetActorRotation(Rotat);
		} }
		if (hour >= 19) { if (hour < 20) { Sun->SetIntensity(20.f - (1.0f * (min / 60.0f))); 
		Rotat.Pitch = (351.0f + (9.0f * (min / 60.0f))); SetActorRotation(Rotat);
		} }
		if (hour >= 20) { Sun->SetIntensity(1.0f); 
		Rotat.Pitch = 360.0f; SetActorRotation(Rotat);
		
		}

		SetSunHeight();
	}
}

//Sun height is a property in a blueprint/editor class; we calculate a value here for it to grab.
//this effectively changes the background, sun location, and color of the "sky sphere' during runtime 
void ADayNightCycle::SetSunHeight()
{
	// -.9 = last dark, .31 = last pink, 1.o = full height/ -.23(sunrise/sunset)
	if (hour < 4) { float x = -1.0f; SunHeight = x; }
	if (hour >= 4) { if (hour < 5) { float x = (-1.0f + (.8f * (min / 60.0f)));SunHeight = x; } }
	if (hour >= 5) { if (hour < 12) { float x = (-.2f + (1.2f*((hour-5.0f)/7.0f)) + (1.2f * ((min / 60.0f) / 7.0f))); SunHeight = x;} }
	if (hour >= 12) { if (hour < 20) { float x = (1.0f - (1.2f*((hour-12.0f)/8.0f)) - (1.2f * ((min / 60.0f) / 8.0f)));SunHeight = x; } }
	if (hour >= 20) { if (hour < 21) { float x = (-.8f - (.1f * ((min / 60.0f))));SunHeight = x; } }
	if (hour > 21) { float x = -1.0f; }
}