// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "enemyHandler.h"
#include "Engine.h"
#include "enemyBaseClass.generated.h"

UCLASS()
class VALH_API AenemyBaseClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AenemyBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Handler")
		AenemyHandler* theHandler;

	UFUNCTION(BlueprintCallable, Category = "Management")
		virtual void OnSpawn(AenemyHandler* theOne);

	void DamageTarget(float damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStats")
		int globalIdentifier;

	UFUNCTION(BlueprintCallable, Category = "BaseStats")
		void SetGlobalID(int ID);

	UFUNCTION()
		void EnemyPushBack();

	UFUNCTION()
		void UpdateReceived();

	UFUNCTION()
		void DeathDrop();

	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshBody;

	UPROPERTY(EditAnywhere, Category = Body)
		class UStaticMesh* bodyDummy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = capsule)
		class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = Movement)
		 FVector primaryNextLocation;

	UFUNCTION()
	void FindNextDestination();

	int State;
	int StateTimer;

	void ClipToGround();

	void RandomRotation();
	void NewState();
	void PlayerDistanceCheck();
	void RunFromPlayer();
	void Aggress();
	void RangedAttack();
	void WalkFunction();

	int Speed;

	ACharacter* playerCharacterReference;

	int walkingSpeed;
	int walkingTimer;
	int runningSpeed;
	int runningTimer;
	int waitingTimer;
	int distanceThreshold;
	int aggressive;
	int damagetype;
	int damage;
	int firerate;
	int AttackCountdown;

	//Gun Flash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		TSubclassOf<AActor> GunFlash;

	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetStats(int walkspeed, int walktimer, int runspeed, int runtimer, int waittimer, int distanceThreshint, int Aggressive, int DamageType, int Damage, int FireRate);

};
