// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "streumon_fpsGameMode.h"
#include "SO_PlayerController.generated.h"

//For client knowledge purpose

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnAbilityStateChangedDelegate, bool, abilityState );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnScoreUpdateDelegate, const TArray< FString >&, names, const TArray< int >&, scores );

/** Custom PlayerController for the game.
 *  Handle the client / server communications.
 *  TODO : add an AInfo to handle client / server communications.
 */
UCLASS()
class STREUMON_FPS_API ASO_PlayerController : public APlayerController
{
	GENERATED_BODY()

	// ATTRIBUTES
public:
	/** Used to relay the wallrun state */
	UPROPERTY( BlueprintAssignable )
	FOnAbilityStateChangedDelegate OnWallrunStateReceivedDelegate;
	
	/** Used to relay the aim assist state */
	UPROPERTY( BlueprintAssignable )
	FOnAbilityStateChangedDelegate OnAimAssistStateReceivedDelegate;
	
	/** Used to relay the scoreboard */
	UPROPERTY( BlueprintAssignable )
	FOnScoreUpdateDelegate OnScoreUpdateDelegate;

	// METHODS
public:
	/** Relay the wallrun state */
	UFUNCTION()
	void OnWallrunStateReceived( bool newWallrunState );

	/** Relay the aim assist state */
	UFUNCTION()
	void OnAimAssistStateReceived( bool newAimAssistState );

	/** Relay the scoreboard */
	UFUNCTION( Client, Reliable )
	void OnScoreUpdate( const TArray< FString >& names, const TArray< int >& scores );
	void OnScoreUpdate_Implementation( const TArray< FString >& names, const TArray< int >& scores );

	/** Send score Update to server */
	UFUNCTION( Server, Reliable )
	void IncrementScore();
	void IncrementScore_Implementation();
};