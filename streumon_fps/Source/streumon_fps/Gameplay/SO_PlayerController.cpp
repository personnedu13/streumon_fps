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

void ASO_PlayerController::OnScoreUpdate_Implementation( const TArray< FString >& names, const TArray< int >& scores )
{
	OnScoreUpdateDelegate.Broadcast( names, scores );
}

void ASO_PlayerController::IncrementScore_Implementation()
{
	if ( Astreumon_fpsGameMode* gameModeSO = Cast< Astreumon_fpsGameMode>( GetWorld()->GetAuthGameMode() ); IsValid( gameModeSO ) )
	{
		gameModeSO->IncrementScore( this );
	}
}