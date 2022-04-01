// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SO_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnAbilityStateChangedDelegate, bool, abilityState );

/**
 * 
 */
UCLASS()
class STREUMON_FPS_API ASO_PlayerController : public APlayerController
{
	GENERATED_BODY()

	// ATTRIBUTES
public:
	UPROPERTY( BlueprintAssignable )
	FOnAbilityStateChangedDelegate OnWallrunStateReceivedDelegate;
	
	UPROPERTY( BlueprintAssignable )
	FOnAbilityStateChangedDelegate OnAimAssistStateReceivedDelegate;

	// METHODS
public:
	UFUNCTION( Client, Reliable )
	void OnWallrunStateReceived( bool newWallrunState );
	void OnWallrunStateReceived_Implementation( bool newWallrunState );

	UFUNCTION( Client, Reliable )
	void OnAimAssistStateReceived( bool newAimAssistState );
	void OnAimAssistStateReceived_Implementation( bool newAimAssistState );
};
