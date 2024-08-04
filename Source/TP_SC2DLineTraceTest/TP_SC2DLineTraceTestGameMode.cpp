// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_SC2DLineTraceTestGameMode.h"
#include "TP_SC2DLineTraceTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATP_SC2DLineTraceTestGameMode::ATP_SC2DLineTraceTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
