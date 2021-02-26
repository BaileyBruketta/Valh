// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enemyBaseClass.h"
#include "NPCBaseClass.generated.h"

/**
 * 
 */
UCLASS()
class VALH_API ANPCBaseClass : public AenemyBaseClass
{
	GENERATED_BODY()

public:
	void ChangeMesh(USkeletalMesh* MeshIn);
	virtual void OnSpawn(AenemyHandler* theOne) override;
	void SABP(UAnimBlueprint* animbp);
	//void OnSpawn2(AMyNPCHandler* theOne);
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Handler")
	//	AMyNPCHandler* theHandler2;
};
