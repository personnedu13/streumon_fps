// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../streumon_fps_UserWidget.h"
#include "Components/VerticalBox.h"
#include "ScoreboardWidget.generated.h"

/**
 *  Main scoreboard widget, use a blueprint made from UScoreboardEntryWidget as lines for the scores.
 */
UCLASS()
class STREUMON_FPS_API UScoreboardWidget : public Ustreumon_fps_UserWidget
{
	GENERATED_BODY()

	// ATTRIBUTES
protected:

	/** The blueprint used to construct the lines of the scoreboard. */
	UPROPERTY( EditAnywhere, Category = "Scoreboard" )
	TSubclassOf<class UScoreboardEntryWidget> EntryClass;

	/** The Panel used to store the widgets created. */
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UVerticalBox* ScoreHolder;

	// METHODS
protected:
	virtual void SynchronizeProperties() override;

public:
	/** Used to received the informations. */
	UFUNCTION()
	void OnScoreDataReceived( const TArray< FString >& names, const TArray< int >& scores );
};