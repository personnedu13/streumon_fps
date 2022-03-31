// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInfoMemberWidget.h"
#include "AbilityInfoMemberWidget.h"
#include "GameFramework/PlayerInput.h"
#include "AbilityInfoWidget.h"

void UAbilityInfoMemberWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();
	if ( GetOwningPlayer() && GetOwningPlayer()->PlayerInput )
	{
		auto keyArray = GetOwningPlayer()->PlayerInput->GetKeysForAction( FName( *AbilityInputName ) );
		if ( keyArray.Num() > 0 )
		{
			KeyText->SetText( keyArray[0].Key.GetDisplayName() );
		}
	}
}

void UAbilityInfoMemberWidget::SynchronizeProperties()
{
	Ustreumon_fps_UserWidget::SynchronizeProperties();
	AbilityImage->SetBrushFromAsset( isAbilityActive ? ActiveAbilityIcon : UnactiveAbilityIcon );
}

bool UAbilityInfoMemberWidget::IsAbilityActive()
{
	return isAbilityActive;
}

void UAbilityInfoMemberWidget::SetAbilityActive( bool isActive )
{
	if ( isAbilityActive != isActive )
	{
		isAbilityActive = isActive;
		AbilityImage->SetBrushFromAsset( isAbilityActive ? ActiveAbilityIcon : UnactiveAbilityIcon );
	}
}