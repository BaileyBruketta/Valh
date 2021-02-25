// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "FoliageGod.generated.h"

UCLASS()
class VALH_API AFoliageGod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoliageGod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant0;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant1;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant2;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant3;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant4;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant5;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant6;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant7;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant8;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant9;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock0;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock1;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock2;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock3;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> rock4;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant10;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant11;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant12;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> plant13;

	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh0;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh1;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh2;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh3;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh4;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh5;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh6;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh7;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh8;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh9;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh10;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh11;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh12;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh13;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh14;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh15;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh16;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh17;
	UPROPERTY(EditDefaultsOnly, Category = "Meshing")
		class UStaticMesh* MyMesh18;

	UPROPERTY(EditDefaultsOnly, Category = "Foliage")
		TArray<UStaticMesh*> Meshes;

	TArray<UInstancedStaticMeshComponent*> MeshComponents;

	UInstancedStaticMeshComponent* meshComponent0;
	UInstancedStaticMeshComponent* meshComponent1;
	UInstancedStaticMeshComponent* meshComponent2;
	UInstancedStaticMeshComponent* meshComponent3;
	UInstancedStaticMeshComponent* meshComponent4;
	UInstancedStaticMeshComponent* meshComponent5;
	UInstancedStaticMeshComponent* meshComponent6;
	UInstancedStaticMeshComponent* meshComponent7;
	UInstancedStaticMeshComponent* meshComponent8;
	UInstancedStaticMeshComponent* meshComponent9;
	UInstancedStaticMeshComponent* meshComponent10;
	UInstancedStaticMeshComponent* meshComponent11;
	UInstancedStaticMeshComponent* meshComponent12;
	UInstancedStaticMeshComponent* meshComponent13;
	UInstancedStaticMeshComponent* meshComponent14;
	UInstancedStaticMeshComponent* meshComponent15;
	UInstancedStaticMeshComponent* meshComponent16;
	UInstancedStaticMeshComponent* meshComponent17;
	UInstancedStaticMeshComponent* meshComponent18;
	

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnFunction();

	TArray<AActor> plants;
	TArray<int> platNumber;
	TArray<FVector> plantLocations;
	TArray<float> scaleMultiplier;
	TArray<int> scaleMax;

	TArray<FString> GetBlockCoords(int BlockNumber);
	void GeneratePlantData(int Block);
	void SpawnPlantsFromData(int Block);
	TArray<FString> GetSeedData(int BlockNumber);
	void AppendPlantToDataFile(int BlockNumber, TArray<FString> PlantInfo);
	TArray<FString> GetPlantData(int BlockNumber);
	

	AInstancedFoliageActor* Foll;

private:
	TArray<UFoliageInstancedStaticMeshComponent*> InstancedStaticMeshComp;

};
