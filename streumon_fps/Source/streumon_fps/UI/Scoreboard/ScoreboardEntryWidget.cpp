// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardEntryWidget.h"

void UScoreboardEntryWidget::SetPlayerName( FString name )
{
	PlayerNameText->SetText( FText::FromString( name ) );
}

void UScoreboardEntryWidget::SetPlayerScore( int32 score )
{
	PlayerScoreText->SetText( FText::FromString(FString( "   " ) + FString::FromInt(score)));
}