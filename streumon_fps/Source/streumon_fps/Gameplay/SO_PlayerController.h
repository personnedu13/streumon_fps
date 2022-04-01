// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "streumon_fpsGameMode.h"
#include "SO_PlayerController.generated.h"

//For client knowledge purpose

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnAbilityStateChangedDelegate, bool, abilityState );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnScoreUpdateDelegate, const TArray< FString >&, names, const TArray< int >&, scores );

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
	
	UPROPERTY( BlueprintAssignable )
	FOnScoreUpdateDelegate OnScoreUpdateDelegate;

	// METHODS
public:
	//UFUNCTION( Client, Reliable)
	void OnWallrunStateReceived( bool newWallrunState );
	//void OnWallrunStateReceived_Implementation( bool newWallrunState );

	//UFUNCTION( Client, Reliable )
	void OnAimAssistStateReceived( bool newAimAssistState );
	//void OnAimAssistStateReceived_Implementation( bool newAimAssistState );

	UFUNCTION( Client, Reliable )
	void OnScoreUpdate( const TArray< FString >& names, const TArray< int >& scores );
	void OnScoreUpdate_Implementation( const TArray< FString >& names, const TArray< int >& scores );

	UFUNCTION( Server, Reliable )
	void IncrementScore();
	void IncrementScore_Implementation();
};