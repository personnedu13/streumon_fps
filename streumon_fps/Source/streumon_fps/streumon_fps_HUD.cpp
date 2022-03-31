// Fill out your copyright notice in the Description page of Project Settings.


#include "streumon_fps_HUD.h"
#include "streumon_fps_UserWidget.h"

void Astreumon_fps_HUD::InitHUD()
{
	WidgetList.Empty();

	for ( TSubclassOf<Ustreumon_fps_UserWidget> WidgetClass : WidgetClassList )
	{
		if ( WidgetClass )
		{
			Ustreumon_fps_UserWidget* newEntry = CreateWidget<Ustreumon_fps_UserWidget>( GetWorld(), WidgetClass );
			if ( newEntry )
			{
				newEntry->AddToViewport();
				WidgetList.Add( newEntry );
				//newEntry->SetupDelegateToObject( this );
			}
		}
	}
}

void Astreumon_fps_HUD::BeginPlay()
{
	InitHUD();
}
