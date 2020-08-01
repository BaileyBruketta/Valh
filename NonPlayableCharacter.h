// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NonPlayableCharacter.generated.h"

UCLASS()
class VALH_API ANonPlayableCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANonPlayableCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		class USkeletalMesh* DefaultMaleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		class USkeletalMesh* DefaultFemaleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
		class USkeletalMeshComponent* NPCBody;

	void InitiatePrebbuiltCharacter(int CharacterNumber);

};
