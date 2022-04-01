// Fill out your copyright notice in the Description page of Project Settings.


#include "streumon_fps_UserWidget.h"

int32 Ustreumon_fps_UserWidget::GetDesiredZOrderInViewport() const
{
    return DesiredZOrderInViewport;
}

void Ustreumon_fps_UserWidget::AddToViewport()
{
    UUserWidget::AddToViewport( DesiredZOrderInViewport );
}