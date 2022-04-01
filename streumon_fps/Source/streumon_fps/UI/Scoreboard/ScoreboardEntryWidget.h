// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../streumon_fps_UserWidget.h"
#include "Components/TextBlock.h"
#include "ScoreboardEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class STREUMON_FPS_API UScoreboardEntryWidget : public Ustreumon_fps_UserWidget
{
	GENERATED_BODY()

	// ATTRIBUTES
protected:

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UTextBlock* PlayerNameText;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UTextBlock* PlayerScoreText;

	// METHODS
public:

	UFUNCTION( BlueprintCallable )
	void SetPlayerName( FString name );

	UFUNCTION( BlueprintCallable )
	void SetPlayerScore( int32 score );
};