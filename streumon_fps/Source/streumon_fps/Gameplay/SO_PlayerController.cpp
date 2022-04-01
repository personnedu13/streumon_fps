// Fill out your copyright notice in the Description page of Project Settings.


#include "SO_PlayerController.h"
#include "../UI/streumon_fps_HUD.h"

void ASO_PlayerController::OnWallrunStateReceived( bool newWallrunState )
{
	OnWallrunStateReceivedDelegate.Broadcast( newWallrunState );
}

void ASO_PlayerController::OnAimAssistStateReceived( bool newAimAssistState )
{
	OnAimAssistStateReceivedDelegate.Broadcast( newAimAssistState );
}

/*void ASO_PlayerController::BeginPlay()
{
	APlayerController::BeginPlay();
	if ( auto SO_HUD = GetHUD<Astreumon_fps_HUD>(); IsValid( SO_HUD ) )
	{
		SO_HUD->OnPlayerControllerBeginPlay();
	}
}*/

void ASO_PlayerController::OnPossess( APawn* aPawn )
{
	APlayerController::OnPossess(aPawn);
	/*if ( auto SO_HUD = GetHUD<Astreumon_fps_HUD>(); IsValid(SO_HUD) )
	{
		SO_HUD->OnPlayerControllerBeginPlay();
	}*/
}
