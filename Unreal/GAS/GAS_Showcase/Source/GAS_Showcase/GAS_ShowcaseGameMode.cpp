// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_ShowcaseGameMode.h"
#include "GAS_ShowcaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAS_ShowcaseGameMode::AGAS_ShowcaseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
