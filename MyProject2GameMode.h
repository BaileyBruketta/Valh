// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProject2GameMode.generated.h"

UCLASS(minimalapi)
class AMyProject2GameMode : public AGameModeBase
{
	GENERATED_BODY()

		//virtual void BeginPlay() override; //overrride beginplay 

public:
	AMyProject2GameMode();

//protected:

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (BlueprintProtected = "true"))
	//TSubclassOf<class UUserWidget> PlayerHUDClass;

	//UPROPERTY()
	//class UUserWidget* CurrentWidget;

};



