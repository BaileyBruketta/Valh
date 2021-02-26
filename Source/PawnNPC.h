// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"
#include "enemyHandler.h"
#include "PawnNPC.generated.h"

UCLASS()
class VALH_API APawnNPC : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ChangeMesh(USkeletalMesh* MeshIn);

	virtual void OnSpawn(AenemyHandler* theOne);

	void SABP(UAnimBlueprint* animbp);

	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Handler")
		AenemyHandler* theHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = capsule)
		class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = movement)
		class UFloatingPawnMovement* movement;

	ACharacter* playerCharacterReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStats")
		int globalIdentifier;

	UFUNCTION(BlueprintCallable, Category = "BaseStats")
		void SetGlobalID(int ID);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	//	FVector* NextLocation();

};
