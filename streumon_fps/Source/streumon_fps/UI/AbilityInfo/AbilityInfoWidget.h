// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../streumon_fps_UserWidget.h"
#include "Components/HorizontalBox.h"
#include "AbilityInfoWidget.generated.h"

/**
 * Main widget for the ability information display.
 * Uses UAbilityInfoMemberWidget to store and display informations.
 */
UCLASS()
class STREUMON_FPS_API UAbilityInfoWidget : public Ustreumon_fps_UserWidget
{
	GENERATED_BODY()
	
	// ATTRIBUTES
protected:
	/** Used to align the ability images. */
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UHorizontalBox* AbilityHolder;

	/** Wallrun widget */
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	class UAbilityInfoMemberWidget* WallrunWidget;

	/** Aim assist widget */
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	class UAbilityInfoMemberWidget* AimAssistWidget;

	// METHODS
protected:
	virtual void SynchronizeProperties() override;

public:
	/** Used to subscribe to the delegate in the PlayerController. */
	UFUNCTION()
	void OnWallrunStateReceived( bool newWallrunState );

	/** Used to subscribe to the delegate in the PlayerController. */
	UFUNCTION()
	void OnAimAssistStateReceived( bool newAimAssistState );
};
