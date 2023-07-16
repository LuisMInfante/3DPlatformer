// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"


void UOverheadWidget::NativeDestruct() 
{
    RemoveFromParent();
    Super::NativeDestruct();
}

void UOverheadWidget::SetDisplayText(FString TextToDisplay) 
{
    if (DisplayText)
    {
        DisplayText->SetText(FText::FromString(TextToDisplay));
    }
}

void UOverheadWidget::ShowPlayerNetworkRole(APawn* InPawn) 
{
    ENetRole RemoteRole = InPawn->GetRemoteRole();
    FString Role;
    switch (RemoteRole)
    {
        case ENetRole::ROLE_Authority:
            Role = FString("Authority");
            break;
        case ENetRole::ROLE_SimulatedProxy:
            Role = FString("Simulated Proxy");
            break;
        case ENetRole::ROLE_AutonomousProxy:
            Role = FString("Autonomous Proxy");
            break;
        case ENetRole::ROLE_None:
            Role = FString("NONE");
            break;
    }

    FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
    SetDisplayText(RemoteRoleString);
}

void UOverheadWidget::ShowPlayerName(APawn* InPawn) 
{
    FString PlayerName = InPawn->GetPlayerState()->GetPlayerName();
    SetDisplayText(PlayerName);
}
