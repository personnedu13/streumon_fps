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

protected:
	UPROPERTY( EditDefaultsOnly, Category = "Widget" )
	TArray<TSubclassOf<class Ustreumon_fps_UserWidget>> WidgetClassList;

	UPROPERTY( BlueprintReadOnly, Category = "Widget" )
	TArray<Ustreumon_fps_UserWidget*> WidgetList;

public:
	Astreumon_fps_HUD() = default;

protected:
	virtual void InitHUD();

protected:
	virtual void BeginPlay() override;
};
