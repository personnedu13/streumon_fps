// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../streumon_fps_UserWidget.h"
#include "Components/VerticalBox.h"
#include "ScoreboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class STREUMON_FPS_API UScoreboardWidget : public Ustreumon_fps_UserWidget
{
	GENERATED_BODY()

	// ATTRIBUTES
protected:

	UPROPERTY( EditAnywhere, Category = "Scoreboard" )
	TSubclassOf<class UScoreboardEntryWidget> EntryClass;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UVerticalBox* ScoreHolder;

	// METHODS
protected:
	virtual void SynchronizeProperties() override;

public:
	UFUNCTION()
	void OnScoreDataReceived( const TArray< FString >& names, const TArray< int >& scores );
};