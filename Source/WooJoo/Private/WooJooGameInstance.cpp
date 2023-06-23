// Fill out your copyright notice in the Description page of Project Settings.


#include "WooJooGameInstance.h"
#include "OnlineSubsystem.h"

UWooJooGameInstance::UWooJooGameInstance()
{

}

void UWooJooGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		FName SubsystemName = Subsystem->GetSubsystemName();
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Online Subsystem : %s"), *SubsystemName.ToString())); }
		bUseLAN = (SubsystemName == FName("NULL"));

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UWooJooGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UWooJooGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UWooJooGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UWooJooGameInstance::OnJoinSessionComplete);
		}
	}
}

void UWooJooGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		WG_TEXT("Create Session Success!");
		WG_LOG("Create Session Success!");
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Session Name : %s"), *SessionName.ToString())); }
		GetWorld()->ServerTravel("/Game/Maps/LV_BoardGame?listen");
	}
	else
	{
		WG_TEXT("Create Session Failed!");
		WG_LOG("Create Session Failed!");
	}
}

void UWooJooGameInstance::OnDestroySessionComplete(FName SessionName, bool Succeeded)
{
	WG_TEXT("Session Destroyed!");
	WG_LOG("Session Destroyed");
	if (Succeeded)
	{
		//...
	}
}

void UWooJooGameInstance::OnFindSessionsComplete(bool Succeeded)
{
	if (Succeeded && SessionSearch.IsValid())
	{
		WG_TEXT("Find Session Success!");
		WG_LOG("Find Session Success!");
		FoundResults = SessionSearch->SearchResults;
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Search Results Num : %d"), FoundResults.Num())); }
	}
	else
	{
		WG_TEXT("Find Session Failed!");
		WG_LOG("Find Session Failed!");
	}
}

void UWooJooGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString JoinAddress;
	if (!SessionInterface->GetResolvedConnectString(SessionName, JoinAddress))
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Join Adress : %s"), *JoinAddress)); }
		APlayerController* PlayerController = GetFirstLocalPlayerController();
		PlayerController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		WG_TEXT("Join Session Success!");
		WG_LOG("Join Session Success!");
	}
	else
	{
		WG_TEXT("Join Session Failed!");
		WG_LOG("Join Session Failed!");
	}
}

void UWooJooGameInstance::CreateServer()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsLANMatch = bUseLAN;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;

	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);

	WG_TEXT("Start to Create Session......");
	WG_LOG("Start to Create Session......");
}

void UWooJooGameInstance::FindServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = bUseLAN;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

		WG_TEXT("Start to Find Session......");
		WG_LOG("Start to Find Session......");
	}
	else
	{
		WG_TEXT("Failed to Find Session.");
		WG_LOG("Failed to Find Session.");
	}
}

void UWooJooGameInstance::JoinServer()
{
	if (FoundResults.Num())
	{
		SessionInterface->JoinSession(0, FName("My Session"), FoundResults[0]);
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Session ID : %s"), *FoundResults[0].GetSessionIdStr())); }
		WG_TEXT("Starting to Join Session......");
		WG_LOG("Starting to Join Session......");
	}
	else 
	{
		WG_TEXT("Failed to Join Session");
		WG_LOG("Failed to Join Session");
	}
}