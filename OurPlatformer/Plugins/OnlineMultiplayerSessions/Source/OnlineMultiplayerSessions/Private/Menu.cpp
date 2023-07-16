// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath) 
{
    PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
    NumPublicConnections = NumberOfPublicConnections;
    MatchType = TypeOfMatch;

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

    // Retrieve custom Multiplayer Subsystem through Game Instance
    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }

    // Bind Callbacks to Custom Delegates
    if (MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
        MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
        MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
        MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
        MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
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

void UMenu::NativeDestruct() 
{
    ClearMenu(); // Remove widget from viewport

	Super::NativeDestruct(); // Deconstruct widget
}

void UMenu::OnCreateSession(bool bWasSuccessful) 
{
	if (bWasSuccessful)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                12.f,
                FColor::Green,
                FString(TEXT("Session was successfully created."))
            );
        }

        // Travel to lobby level after successful creation of session
        UWorld* World = GetWorld();
        if (World)
        {
            World->ServerTravel(PathToLobby); // testing for now

            APlayerController* PlayerController = World->GetFirstPlayerController();
            if (PlayerController)
            {
                FInputModeGameOnly InputModeData;
                PlayerController->SetInputMode(InputModeData);
                PlayerController->SetShowMouseCursor(false);
            }
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                12.f,
                FColor::Red,
                FString(TEXT("Session creation failed."))
            );
        }

        HostButton->SetIsEnabled(true);
    }
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful) 
{
    if (!MultiplayerSessionsSubsystem)
    {
        return;
    }

	// Parse Search Results
    // * TODO: Refine Selection of Session * 
    for (auto Result : SessionResults)
    {
        FString SessionSettingsValue;
        Result.Session.SessionSettings.Get(FName("MatchType"), SessionSettingsValue);

        // If the result matches our Matchtype, join that session
        if (SessionSettingsValue == MatchType)
        {
            MultiplayerSessionsSubsystem->JoinSession(Result);
            return;
        }
    }

    if (!bWasSuccessful || SessionResults.Num() == 0)
    {
        JoinButton->SetIsEnabled(true);
    }
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result) 
{
	// Get Online subsystem (GameEngine.ini)
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            FString Address;
            SessionInterface->GetResolvedConnectString(NAME_GameSession, Address); // Get the Address

            APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
            if (PlayerController)
            {
                PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute); // Travel to that Address
                FInputModeGameOnly InputModeData;
                PlayerController->SetInputMode(InputModeData);
                PlayerController->SetShowMouseCursor(false);
            }
        }
    }

    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        JoinButton->SetIsEnabled(true);
    }
}

void UMenu::OnStartSession(bool bWasSuccessful) 
{
	// Travel to Overworld level after successful creation of session
    
    // UWorld* World = GetWorld();
    // if (World)
    // {
    //     World->ServerTravel("/Game/Maps/PlatformerMapTest?listen"); // testing for now

    //     APlayerController* PlayerController = World->GetFirstPlayerController();
    //     if (PlayerController)
    //     {
    //         FInputModeGameOnly InputModeData;
    //         PlayerController->SetInputMode(InputModeData);
    //         PlayerController->SetShowMouseCursor(false);
    //     }
    // }
}

void UMenu::OnDestroySession(bool bWasSuccessful) 
{
	if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            12.f,
            FColor::Purple,
            FString(TEXT("Session Successfully Destroyed."))
        );
    }
}

void UMenu::HostButtonClicked() 
{
    HostButton->SetIsEnabled(false);

    if (MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
    }
}

void UMenu::JoinButtonClicked() 
{
    JoinButton->SetIsEnabled(false);

    if (MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->FindSessions(10000);
    }
}

void UMenu::ClearMenu() 
{
	RemoveFromParent();
    // UWorld* World = GetWorld();
    // if (World)
    // {
    //     APlayerController* PlayerController = World->GetFirstPlayerController();
    //     if (PlayerController)
    //     {
    //         FInputModeGameOnly InputModeData;
    //         PlayerController->SetInputMode(InputModeData);
    //         PlayerController->SetShowMouseCursor(false);
    //     }
    // }
}