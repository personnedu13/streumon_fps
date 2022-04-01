// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardWidget.h"
#include "ScoreboardEntryWidget.h"
#include "../../Gameplay/SO_PlayerController.h"
#include "Blueprint/WidgetTree.h"

void UScoreboardWidget::SynchronizeProperties()
{
	Ustreumon_fps_UserWidget::SynchronizeProperties();

	// Setup delegates
	if ( auto playerController = GetOwningPlayer<ASO_PlayerController>(); IsValid( playerController ) )
	{
		playerController->OnScoreUpdateDelegate.AddDynamic( this, &UScoreboardWidget::OnScoreDataReceived );
	}
}

void UScoreboardWidget::OnScoreDataReceived( const TArray< FString >& names, const TArray< int >& scores )
{
	// Create map
	TMap< FString, int> playerScores;
	for ( int i = 0; i < names.Num(); i++ )
	{
		playerScores.Add( names[i], scores[i] );
	}

	int32 index = 0;
	int32 existentEntryNum = ScoreHolder->GetChildrenCount();
	for ( TPair< FString, int32 > individualScore : playerScores )
	{
		if ( index < existentEntryNum )
		{
			if ( UScoreboardEntryWidget* entry = Cast<UScoreboardEntryWidget>( ScoreHolder->GetChildAt( index ) ); IsValid( entry ) )
			{
				entry->SetPlayerName( individualScore.Key );
				entry->SetPlayerScore( individualScore.Value );
			}
		}
		else
		{
			auto newEntry = WidgetTree->ConstructWidget<UScoreboardEntryWidget>( EntryClass );
			if ( IsValid( newEntry ) )
			{
				newEntry->SetPlayerName( individualScore.Key );
				newEntry->SetPlayerScore( individualScore.Value );
				auto slot = ScoreHolder->AddChild( newEntry );
			}
		}
		index++;
	}

	for ( ; index < existentEntryNum; existentEntryNum-- )
	{
		ScoreHolder->RemoveChildAt( index );
	}
}