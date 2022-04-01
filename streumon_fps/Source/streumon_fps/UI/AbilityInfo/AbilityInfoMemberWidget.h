// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../streumon_fps_UserWidget.h"
#include "Slate/SlateBrushAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AbilityInfoMemberWidget.generated.h"

/**
 * Widget used for AbilityInfoWidget.
 */
UCLASS()
class STREUMON_FPS_API UAbilityInfoMemberWidget : public Ustreumon_fps_UserWidget
{
	GENERATED_BODY()

	// ATTRIBUTES

protected:
	/** Image of the ability. */
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UImage* AbilityImage;

	/** Key name to help the user.
	 *  Found with AbilityInputName.
	 */
	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UTextBlock* KeyText;

	/** Image used when the ability is active. */
	UPROPERTY(EditAnywhere)
	USlateBrushAsset* ActiveAbilityIcon;

	/** Image used when the ability is unactive. */
	UPROPERTY(EditAnywhere)
	USlateBrushAsset* UnactiveAbilityIcon;

	/** Name of the Input in the project setting. 
	 *  Used to determine the text to display below the ability.
	 */
	UPROPERTY( EditAnywhere )
	FString AbilityInputName = "Dummy";

	/** I dont think I need to explain this one. */
	UPROPERTY()
	bool isAbilityActive = true;

	// METHODS
protected:

	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;

	// GETTER
public:
	UFUNCTION( BlueprintCallable )
	bool IsAbilityActive();

	// SETTER
	UFUNCTION( BlueprintCallable )
	void SetAbilityActive( bool isActive );
};
