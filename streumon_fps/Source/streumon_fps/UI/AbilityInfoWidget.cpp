// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInfoWidget.h"
#include "AbilityInfoMemberWidget.h"

void UAbilityInfoWidget::SynchronizeProperties()
{
	Ustreumon_fps_UserWidget::SynchronizeProperties();

	// Insert padding between abilities
	UUserWidget* userWidget = nullptr;
	UUserWidget* nextUserWidget = nullptr;
	for ( int32 i = 0; i < AbilityHolder->GetAllChildren().Num(); i++ )
	{
		nextUserWidget = userWidget;
		userWidget = Cast<UUserWidget>( AbilityHolder->GetAllChildren()[i] );
		if ( IsValid( userWidget ) && IsValid( nextUserWidget ) && i > 0 )
		{
			userWidget->SetPadding( FMargin( 8.0f, 0.0f, 0.0f, 0.0f ) );
			nextUserWidget->SetPadding( FMargin( 0.0f, 0.0f, 8.0f, 0.0f ) );
		}
	}
}
