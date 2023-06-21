// Fill out your copyright notice in the Description page of Project Settings.


#include "WooJooGameInstance.h"
#include "OnlineSubsystem.h"
//#include <Kismet/GameplayStatics.h>

UWooJooGameInstance::UWooJooGameInstance()
{

}

void UWooJooGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SubsystemName = Subsystem->GetSubsystemName();
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Online Subsystem : %s"), *SubsystemName.ToString())); }

		/* 플러그인으로 대체함
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UWooJooGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UWooJooGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UWooJooGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UWooJooGameInstance::OnJoinSessionComplete);
		}
		*/
	}
}



/* 플러그인으로 대체함
void UWooJooGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		WG_TEXT("Create Session Success!");
		WG_LOG("Create Session Success!");
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Session Name : %s"), *SessionName.ToString())); }
		GetWorld()->ServerTravel("/Game/Sample/VRTemplate/Maps/VRTemplateMap?listen");
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
		CreateServer();
	}
}

void UWooJooGameInstance::OnFindSessionsComplete(bool Succeeded)
{
	if (Succeeded && SessionSearch.IsValid())
	{
		WG_TEXT("Find Session Success!");
		WG_LOG("Find Session Success!");
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Search Results Num : %d"), SearchResults.Num())); }
		if (SearchResults.Num())
		{
			SessionInterface->JoinSession(0, FName("My Session"), SearchResults[0]);
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Session ID : %s"), *SearchResults[0].GetSessionIdStr())); }
			WG_TEXT("Starting to Join Session......");
			WG_LOG("Starting to Join Session......");
		}
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
		//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
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
	SessionSettings.bIsLANMatch = (SubsystemName == "NULL");
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 3;
	SessionSettings.bUseLobbiesIfAvailable = true;

	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);

	WG_TEXT("Start to Create Session......");
	WG_LOG("Start to Create Session......");
}

void UWooJooGameInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = (SubsystemName == "NULL");
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
*/