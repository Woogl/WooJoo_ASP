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

void UServerEntry::SetServerInfo(const FOnlineSessionSearchResult& SearchResult)
{
	SessionInfo = SearchResult;
	// TODO: ¼­¹ö Á¤º¸ À§Á¬¿¡ ¶ç¿ì±â

	ServerNameText->SetText(FText::FromString(SessionInfo.Session.OwningUserName));
	//TODO: PlayersText->SetText
}

void UServerEntry::OnJoinSessionClicked()
{
	auto WJGameIns = Cast<UWooJooGameInstance>(GetGameInstance());
	// TODO: ÀÎµ¦½º °í¸£±â
	WJGameIns->JoinServer(1);
}
