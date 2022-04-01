// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "streumon_fpsGameMode.generated.h"

class ASO_PlayerController;

/** 
 * Here is stored the scoreboard.
 */
UCLASS(minimalapi)
class Astreumon_fpsGameMode : public AGameModeBase
{
	GENERATED_BODY()

	// ATTRIBUTES

protected:
	TMap< ASO_PlayerController*, int > playerScore;

	// CONSTRUCTORS

public:
	Astreumon_fpsGameMode();

	// METHODS
public:
	/** Used to receive increment for projectile hit. */
	UFUNCTION( Server, Reliable )
	void IncrementScore( ASO_PlayerController* playerController );
	void IncrementScore_Implementation( ASO_PlayerController* playerController );
};