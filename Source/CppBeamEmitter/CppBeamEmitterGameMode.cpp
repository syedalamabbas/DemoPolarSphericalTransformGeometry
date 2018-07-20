// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CppBeamEmitterGameMode.h"
#include "CppBeamEmitter.h"
#include "CppBeamEmitterHUD.h"
#include "CppBeamEmitterCharacter.h"

ACppBeamEmitterGameMode::ACppBeamEmitterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACppBeamEmitterHUD::StaticClass();
}
