// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerEntry.h"
#include <UMG/Public/Components/Button.h>
#include "WooJooGameInstance.h"
#include <UMG/Public/Components/TextBlock.h>

bool UServerEntry::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (Button_JoinSession)
	{
		Button_JoinSession->OnClicked.AddDynamic(this, &UServerEntry::OnJoinSessionClicked);
	}
	
	return true;
}

void UServerEntry::Setup(const FOnlineSessionSearchResult& SearchResult, int Index)
{
	SessionInfo = SearchResult;
	ServerIndex = Index;

	ServerNameText->SetText(FText::FromString(SessionInfo.Session.OwningUserName));
	MaxPlayers->SetText(FText::AsNumber(SessionInfo.Session.SessionSettings.NumPublicConnections));
	CurPlayers->SetText(FText::AsNumber(SessionInfo.Session.SessionSettings.NumPublicConnections - SessionInfo.Session.NumOpenPublicConnections));
}

void UServerEntry::OnJoinSessionClicked()
{
	auto WJGameIns = Cast<UWooJooGameInstance>(GetGameInstance());
	WJGameIns->JoinServer(ServerIndex);
}
