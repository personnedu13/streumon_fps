// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "streumon_fps_UserWidget.generated.h"

/**
 *  User widget override to add some feature to the base class.
 */
UCLASS()
class STREUMON_FPS_API Ustreumon_fps_UserWidget : public UUserWidget
{
	GENERATED_BODY()

	// ATTRIBUTES
protected:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "SL_UserWidget" )
	int32 DesiredZOrderInViewport = 0;

	// METHODS
public:
	int32 GetDesiredZOrderInViewport() const;

	virtual void AddToViewport(); // No override to prevent polymorphism
};
