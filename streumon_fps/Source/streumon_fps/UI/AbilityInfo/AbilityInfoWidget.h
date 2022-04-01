// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../streumon_fps_UserWidget.h"
#include "Components/HorizontalBox.h"
#include "AbilityInfoWidget.generated.h"



/*USTRUCT()
struct STREUMON_FPS_API FAbilityIconAsset
{
	GENERATED_USTRUCT_BODY()

	FAbilityIconAsset() = default;

	UPROPERTY( EditAnywhere )
	USlateBrushAsset* ActiveAbilityIcon;

	UPROPERTY( EditAnywhere )
	USlateBrushAsset* UnactiveAbilityIcon;
};*/

/**
 *
 */
UCLASS()
class STREUMON_FPS_API UAbilityInfoWidget : public Ustreumon_fps_UserWidget
{
	GENERATED_BODY()
	
	// ATTRIBUTES
protected:
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UHorizontalBox* AbilityHolder;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	class UAbilityInfoMemberWidget* WallrunWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	class UAbilityInfoMemberWidget* AimAssistWidget;

	// METHODS
protected:
	virtual void SynchronizeProperties() override;

public:
	UFUNCTION( Client, Reliable )
	void OnWallrunStateReceived( bool newWallrunState );
	void OnWallrunStateReceived_Implementation( bool newWallrunState );

	UFUNCTION( Client, Reliable )
	void OnAimAssistStateReceived( bool newAimAssistState );
	void OnAimAssistStateReceived_Implementation( bool newAimAssistState );
};
