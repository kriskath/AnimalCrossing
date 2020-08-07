// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimalCrossingGameMode.h"
#include "AnimalCrossingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAnimalCrossingGameMode::AAnimalCrossingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
