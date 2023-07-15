// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"


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
    ENetRole RemoteRole = InPawn->GetRemotRole();
    FString Role;
    switch (RemotRole)
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

    FString RemotRoleString = FString::Printf(TEXT("Remot Role: %s"), *Role);
    SetDisplayText(RemotRoleString);
}
