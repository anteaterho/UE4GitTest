// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "UE4GitTest.h"
#include "UE4GitTestGameMode.h"
#include "MyHUD.h"
#include "UE4GitTestCharacter.h"

AUE4GitTestGameMode::AUE4GitTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		HUDClass = AMyHUD::StaticClass();
	}
}
