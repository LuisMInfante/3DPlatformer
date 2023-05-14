// Copyright Epic Games, Inc. All Rights Reserved.

#include "OurPlatformerGameMode.h"
#include "OurPlatformerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOurPlatformerGameMode::AOurPlatformerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
