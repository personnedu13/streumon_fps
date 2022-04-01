// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "streumon_fps_HUD.generated.h"

/**
 *  Custom HUD, used to easily add and remove widget from the UI.
 */
UCLASS()
class STREUMON_FPS_API Astreumon_fps_HUD : public AHUD
{
	GENERATED_BODY()

	// ATTRIBUTES
protected:
	/** List of widget blueprint to display. */
	UPROPERTY( EditDefaultsOnly, Category = "Widget" )
	TArray<TSubclassOf<class Ustreumon_fps_UserWidget>> WidgetClassList;

	/** List of widgets added to the UI. */
	UPROPERTY( BlueprintReadOnly, Category = "Widget" )
	TArray<Ustreumon_fps_UserWidget*> WidgetList;

	// CONSTRUCTORS

public:
	Astreumon_fps_HUD() = default;

	// METHODS

protected:
	/** Used to initialize the HUD. */
	virtual void InitHUD();

	virtual void BeginPlay() override;
};
