// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "streumon_fps_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class STREUMON_FPS_API Ustreumon_fps_UserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "SL_UserWidget" )
	int32 DesiredZOrderInViewport = 0;

public:
	int32 GetDesiredZOrderInViewport() const;

	virtual void AddToViewport(); // No override to prevent polymorphism
};
