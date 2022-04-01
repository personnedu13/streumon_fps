// Copyright Epic Games, Inc. All Rights Reserved.

#include "streumon_fpsGameMode.h"
#include "streumon_fpsCharacter.h"
#include "UObject/ConstructorHelpers.h"

Astreumon_fpsGameMode::Astreumon_fpsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
