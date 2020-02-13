// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProject2GameMode.h"
#include "MyProject2HUD.h"
#include "MyProject2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"





//void AMyProject2GameMode::BeginPlay()
//{
//	Super::BeginPlay();

//	if (PlayerHUDClass != nullptr) 
//	{
//		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);

//		if (CurrentWidget != nullptr) 
//		{
//			CurrentWidget->AddToViewport();
	//	}
//	}
//}

AMyProject2GameMode::AMyProject2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMyProject2HUD::StaticClass();
}
