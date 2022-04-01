// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "streumon_fpsGameMode.generated.h"

USTRUCT()
struct FScoreData
{
	GENERATED_USTRUCT_BODY()

	//FString Name;

	UPROPERTY()
	int Score;

	FScoreData() = default;
};

class ASO_PlayerController;

UCLASS(minimalapi)
class Astreumon_fpsGameMode : public AGameModeBase
{
	GENERATED_BODY()

	// ATTRIBUTES

protected:
	TMap< ASO_PlayerController*, uint32 > playerScore;

	// CONSTRUCTORS

public:
	Astreumon_fpsGameMode();

	// METHODS
	UFUNCTION( Server, Reliable )
	void IncrementScore( ASO_PlayerController* playerController );
	void IncrementScore_Implementation( ASO_PlayerController* playerController );
};