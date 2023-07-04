// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"

void UMenu::MenuSetup() 
{
	AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    SetIsFocusable(true);

    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if (PlayerController)
        {
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(true);
        }
    }
}

bool UMenu::Initialize() 
{
    if (!Super::Initialize())
    {
        return false;
    }

    if (HostButton)
    {
        HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
    }

    if (JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
    }

	return true;
}

void UMenu::HostButtonClicked() 
{
	if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            12.f,
            FColor::Magenta,
            FString(TEXT("Host Button Clicked."))
        );
    }
}

void UMenu::JoinButtonClicked() 
{
	if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            12.f,
            FColor::Magenta,
            FString(TEXT("Join Button Clicked."))
        );
    }
}