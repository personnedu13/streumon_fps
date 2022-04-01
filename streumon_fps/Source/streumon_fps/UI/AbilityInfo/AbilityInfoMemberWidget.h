// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../streumon_fps_UserWidget.h"
#include "Slate/SlateBrushAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AbilityInfoMemberWidget.generated.h"

/**
 * 
 */
UCLASS()
class STREUMON_FPS_API UAbilityInfoMemberWidget : public Ustreumon_fps_UserWidget
{
	GENERATED_BODY()

	//friend class AbilityInfoWidget;

	// ATTRIBUTES
protected:

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UImage* AbilityImage;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	UTextBlock* KeyText;

	UPROPERTY(EditAnywhere)
	USlateBrushAsset* ActiveAbilityIcon;

	UPROPERTY(EditAnywhere)
	USlateBrushAsset* UnactiveAbilityIcon;

	UPROPERTY( EditAnywhere )
	FString AbilityInputName = "Dummy";

	UPROPERTY()
	bool isAbilityActive = true;

	virtual void NativeConstruct() override;

	// METHODS
protected:
	virtual void SynchronizeProperties() override;

public:
	UFUNCTION( BlueprintCallable )
	bool IsAbilityActive();

	UFUNCTION( BlueprintCallable )
	void SetAbilityActive( bool isActive );
};
