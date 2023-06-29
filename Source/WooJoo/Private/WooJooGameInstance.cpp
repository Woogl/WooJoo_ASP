// Fill out your copyright notice in the Description page of Project Settings.


#include "WooJooGameInstance.h"
#include <UMG/Public/Blueprint/UserWidget.h>
#include "MainWidget.h"
#include <Kismet/GameplayStatics.h>

UWooJooGameInstance::UWooJooGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainWBPClass(TEXT("/Game/Blueprints/UI/WBP_Main_CPP"));
	if (MainWBPClass.Class)
	{
		MainWidgetClass = MainWBPClass.Class;
	}
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

void UWooJooGameInstance::LoadMainWidget()
{
	MainWidget = CreateWidget<UMainWidget>(this, MainWidgetClass);
	MainWidget->AddToViewport();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetShowMouseCursor(true);
}

void UWooJooGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	SessionInterface->CancelFindSessions();

	if (Succeeded)
	{
		WG_TEXT("Create Session Success!");
		WG_LOG("Create Session Success!");
		if (bIsCounselMode == false)
		{
			GetWorld()->ServerTravel("/Game/Maps/LV_BoardGame?listen");
		}
		else
		{
			GetWorld()->ServerTravel("/Game/Maps/LV_Counsel?listen");
		}
	}
	else
	{
		WG_TEXT("Create Session Failed!");
		WG_LOG("Create Session Failed!");
	}
}

void UWooJooGameInstance::OnDestroySessionComplete(FName SessionName, bool Succeeded)
{
	SessionInterface->CancelFindSessions();

	WG_TEXT("Session Destroyed!");
	WG_LOG("Session Destroyed");
	if (Succeeded)
	{
		CreateServer(bIsCounselMode);
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
		if (MainWidget)
		{
			if (FoundResults.Num() == 0)
			{
				MainWidget->CantFindSession();
			}
			else
			{
				MainWidget->SetServerList(FoundResults);	
			}
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
	SessionInterface->CancelFindSessions();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PlayerController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
			WG_TEXT("Join Session Success!");
			WG_LOG("Join Session Success!");
		}
		WG_TEXT("Join Session Failed!");
		WG_LOG("Join Session Failed!");
	}
	else
	{
		WG_TEXT("Join Session Failed!");
		WG_LOG("Join Session Failed!");
	}
}

void UWooJooGameInstance::CreateServer(bool ShouldCounsel)
{
	SessionInterface->CancelFindSessions();
	bIsCounselMode = ShouldCounsel;

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL"));
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.Set(TEXT("CounselKey"), bIsCounselMode, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	
	WG_TEXT("Try to Create Session......");
	WG_LOG("Try to Create Session......");
}

void UWooJooGameInstance::FindServer()
{
	SessionInterface->CancelFindSessions();

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL"));
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionSearch->QuerySettings.Set(TEXT("CounselKey"), bIsCounselMode, EOnlineComparisonOp::Equals);

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

		WG_TEXT("Start to Find Session......");
		WG_LOG("Start to Find Session......");
	}
	else
	{
		WG_TEXT("Failed to Find Session.");
		WG_LOG("Failed to Find Session.");
	}
	MainWidget->ShowLoadingScreen();
}

void UWooJooGameInstance::JoinServer(int Index)
{
	SessionInterface->CancelFindSessions();

	if (FoundResults.Num())
	{
		SessionInterface->JoinSession(0, NAME_GameSession, FoundResults[Index]);
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

void UWooJooGameInstance::CloseServer()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowInvites = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL"));
	SessionSettings.bShouldAdvertise = false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.Set(TEXT("CounselKey"), bIsCounselMode, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->UpdateSession(NAME_GameSession, SessionSettings);
}
