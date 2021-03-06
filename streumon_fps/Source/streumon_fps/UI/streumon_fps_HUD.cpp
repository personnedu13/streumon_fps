// Fill out your copyright notice in the Description page of Project Settings.


#include "streumon_fps_HUD.h"
#include "streumon_fps_UserWidget.h"

void Astreumon_fps_HUD::InitHUD()
{
	WidgetList.Empty();

	for ( TSubclassOf<Ustreumon_fps_UserWidget> widgetClass : WidgetClassList )
	{
		if ( widgetClass )
		{
			Ustreumon_fps_UserWidget* newEntry = CreateWidget<Ustreumon_fps_UserWidget>( GetWorld(), widgetClass );
			if ( newEntry )
			{
				newEntry->AddToViewport();
				// Will only work for serverside
				WidgetList.Add( newEntry );
			}
		}
	}
}

void Astreumon_fps_HUD::BeginPlay()
{
	InitHUD();
}
