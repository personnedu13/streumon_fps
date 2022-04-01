// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInfoWidget.h"
#include "AbilityInfoMemberWidget.h"
#include "../../Gameplay/SO_PlayerController.h"

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

	// Setup delegates
	if ( auto playerController = GetOwningPlayer<ASO_PlayerController>(); IsValid( playerController ) )
	{
		playerController->OnWallrunStateReceivedDelegate.AddDynamic( WallrunWidget, &UAbilityInfoMemberWidget::SetAbilityActive );
		playerController->OnAimAssistStateReceivedDelegate.AddDynamic( AimAssistWidget, &UAbilityInfoMemberWidget::SetAbilityActive );
	}
}

void UAbilityInfoWidget::OnWallrunStateReceived( bool newWallrunState )
{
	WallrunWidget->SetAbilityActive( newWallrunState );
}

void UAbilityInfoWidget::OnAimAssistStateReceived( bool newAimAssistState )
{
	AimAssistWidget->SetAbilityActive( newAimAssistState );
}

void UAbilityInfoWidget::OnPlayerControllerBeginPlay()
{
	/*if ( auto playerController = GetOwningPlayer<ASO_PlayerController>(); IsValid(playerController) )
	{
		playerController->OnWallrunStateReceivedDelegate.AddDynamic( WallrunWidget, &UAbilityInfoMemberWidget::SetAbilityActive );
		playerController->OnAimAssistStateReceivedDelegate.AddDynamic( AimAssistWidget, &UAbilityInfoMemberWidget::SetAbilityActive );
	}*/
}
