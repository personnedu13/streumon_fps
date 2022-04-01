// Fill out your copyright notice in the Description page of Project Settings.


#include "SO_PlayerController.h"

void ASO_PlayerController::OnWallrunStateReceived_Implementation( bool newWallrunState )
{
	OnWallrunStateReceivedDelegate.Broadcast( newWallrunState );
}

void ASO_PlayerController::OnAimAssistStateReceived_Implementation( bool newAimAssistState )
{
	OnAimAssistStateReceivedDelegate.Broadcast( newAimAssistState );
}