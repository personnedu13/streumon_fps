// Copyright Epic Games, Inc. All Rights Reserved.

#include "streumon_fpsGameMode.h"
#include "streumon_fpsCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "SO_PlayerController.h"

Astreumon_fpsGameMode::Astreumon_fpsGameMode()
{
}

void Astreumon_fpsGameMode::IncrementScore_Implementation( ASO_PlayerController* playerController )
{
	if ( playerScore.Contains( playerController ) )
	{
		playerScore[playerController]++;
	}
	else
	{
		playerScore.Add( playerController, 1 );
	}
	// Notify all player
	for ( auto PCIterator = GetWorld()->GetPlayerControllerIterator(); PCIterator; PCIterator++ )
	{
		if ( ASO_PlayerController* PC = Cast<ASO_PlayerController>( PCIterator->Get() ); PC )
		{
			TArray<FString> names;
			TArray<int> scores;
			for ( auto individualScore : playerScore )
			{
				names.Add( individualScore.Key->GetName() );
				scores.Add( individualScore.Value );
			}
			PC->OnScoreUpdate( names, scores );
		}
	}
}
