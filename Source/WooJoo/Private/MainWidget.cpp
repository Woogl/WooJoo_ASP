// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include <OnlineSessionSettings.h>
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include <UMG/Public/Components/PanelWidget.h>
#include <UMG/Public/Components/Button.h>
#include "WooJooGameInstance.h"
#include "ServerEntry.h"
#include <UMG/Public/Components/TextBlock.h>

UMainWidget::UMainWidget(const FObjectInitializer& ObjectInitializer): UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> SearchingWBPClass(TEXT("/Game/Blueprints/UI/WBP_ServerSearching"));
	if (SearchingWBPClass.Class)
	{
		SearchingWidgetClass = SearchingWBPClass.Class;
	}
	
	ConstructorHelpers::FClassFinder<UUserWidget> NoSessionWBPClass(TEXT("/Game/Blueprints/UI/WBP_NoSession"));
	if (NoSessionWBPClass.Class)
	{
		NoSessionWidgetClass = NoSessionWBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UServerEntry> ServerEntryWBPClass(TEXT("/Game/Blueprints/UI/WBP_ServerEntry"));
	if (ServerEntryWBPClass.Class)
	{
		ServerEntryClass = ServerEntryWBPClass.Class;
	}
}

bool UMainWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (Button_SinglePlay)
	{
		Button_SinglePlay->OnClicked.AddDynamic(this, &UMainWidget::OnClickedSinglePlay);
	}
	if (Button_MultiPlay)
	{
		Button_MultiPlay->OnClicked.AddDynamic(this, &UMainWidget::OnClickedMultiPlay);
	}
	if (Button_Counsel)
	{
		Button_Counsel->OnClicked.AddDynamic(this, &UMainWidget::OnClickedCounsel);
	}
	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UMainWidget::OnClickedQuit);
	}
	if (Button_Host)
	{
		Button_Host->OnClicked.AddDynamic(this, &UMainWidget::OnClickedHost);
	}
	if (Button_Refresh)
	{
		Button_Refresh->OnClicked.AddDynamic(this, &UMainWidget::OnClickedRefresh);
	}
	if (Button_Back)
	{
		Button_Back->OnClicked.AddDynamic(this, &UMainWidget::OnClickedBack);
	}

	return true;
}

void UMainWidget::OnClickedSinglePlay()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->ClientTravel("/Game/Maps/LV_BoardGame", ETravelType::TRAVEL_Absolute);
	}
}

void UMainWidget::OnClickedMultiPlay()
{
	bCounselMode = false;
	Text_Mode->SetText(FText::FromString("Board Game Mode : Server List"));
	WidgetSwitcher->SetActiveWidgetIndex(1);
	auto WJGameIns = Cast<UWooJooGameInstance>(GetGameInstance());
	WJGameIns->FindServer();
}

void UMainWidget::OnClickedCounsel()
{
	bCounselMode = true;
	Text_Mode->SetText(FText::FromString("Counsel Mode : Server List"));
	WidgetSwitcher->SetActiveWidgetIndex(1);
	auto WJGameIns = Cast<UWooJooGameInstance>(GetGameInstance());
	WJGameIns->FindServer();
}

void UMainWidget::OnClickedQuit()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			PlayerController->ConsoleCommand("quit");
		}
	}
}

void UMainWidget::OnClickedHost()
{
	auto WJGameIns = Cast<UWooJooGameInstance>(GetGameInstance());
	WJGameIns->CreateServer(bCounselMode);
}

void UMainWidget::OnClickedRefresh()
{
	auto WJGameIns = Cast<UWooJooGameInstance>(GetGameInstance());
	WJGameIns->FindServer();
}

void UMainWidget::OnClickedBack()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);

	if (auto SessionInterface = IOnlineSubsystem::Get()->GetSessionInterface())
	{
		SessionInterface->CancelFindSessions();
	}
}

void UMainWidget::ShowLoadingScreen()
{
	if (ServerList == nullptr) return;

	ServerList->ClearChildren();
	SearchingWidget = CreateWidget<UUserWidget>(this, SearchingWidgetClass);
	ServerList->AddChild(SearchingWidget);
}

void UMainWidget::SetServerList(TArray<FOnlineSessionSearchResult> SessionSearchResults)
{
	if (ServerList == nullptr) return;

	ServerList->ClearChildren();

	int ServerIndex = 0;
	for (auto SessionSearchResult : SessionSearchResults)
	{
		if (SessionSearchResult.IsValid())
		{
			auto Entry = CreateWidget<UServerEntry>(this, ServerEntryClass);
			ServerList->AddChild(Entry);
			Entry->Setup(SessionSearchResult, ServerIndex);
			ServerIndex++;
		}
	}

	if (ServerIndex == 0)
	{
		CantFindSession();
	}
}

void UMainWidget::CantFindSession()
{
	if (ServerList == nullptr) return;

	ServerList->ClearChildren();
	NoSessionWidget = CreateWidget<UUserWidget>(this, NoSessionWidgetClass);
	ServerList->AddChild(NoSessionWidget);
}