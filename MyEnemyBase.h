// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEnemyBase.generated.h"

UCLASS()
class MYPROJECT2_API AMyEnemyBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEnemyBase();

	//this is what gets dropped when the enemy dies, or what replaces the enemy's body
	UPROPERTY(EditDefaultsOnly, Category = "Mob Loot")
	TSubclassOf<AActor> mobBodyDead;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* TargetMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStats")
		float initHealth;
	void OnDefeatedTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStats")
	float Health;

	UFUNCTION(BlueprintImplementableEvent, Category = "damage")
		void damageEventFunc();

	void DamageTarget(float Damage);
	float EnemyNumber;

};
