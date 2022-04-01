// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "streumon_fps_HUD.generated.h"

/**
 * 
 */
UCLASS()
class STREUMON_FPS_API Astreumon_fps_HUD : public AHUD
{
	GENERATED_BODY()

	// ATTRIBUTES
protected:
	UPROPERTY( EditDefaultsOnly, Category = "Widget" )
	TArray<TSubclassOf<class Ustreumon_fps_UserWidget>> WidgetClassList;

	UPROPERTY( BlueprintReadOnly, Category = "Widget" )
	TArray<Ustreumon_fps_UserWidget*> WidgetList;

	// CONSTRUCTORS

public:
	Astreumon_fps_HUD() = default;

	// METHODS
protected:
	virtual void InitHUD();

	virtual void BeginPlay() override;

public:
	virtual void OnPlayerControllerBeginPlay();
};
