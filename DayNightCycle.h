// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.generated.h"


UCLASS()
class VALH_API ADayNightCycle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayNightCycle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sun)
		class UDirectionalLightComponent* Sun;

	void UpdateTime();
	FTimerHandle TimerHandle;
	void ResetMinutes();
	void UpdateIntensity();

	ACharacter* MyCharacterRef;
	//BP_Sky_Sphere_C* SKySphere;

	int hour;
	float min;
	int sec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sun)
		float SunHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sun)
		float IntensityMultiplier;
	void SetSunHeight();

};
