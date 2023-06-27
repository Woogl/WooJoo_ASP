// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <OnlineSessionSettings.h>
#include "ServerEntry.generated.h"

/**
 * 
 */
UCLASS()
class WOOJOO_API UServerEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnlineSessionSearchResult SessionInfo;

	void SetServerInfo(const FOnlineSessionSearchResult& SearchResult);

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_JoinSession;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayersText;

	UFUNCTION()
	void OnJoinSessionClicked();
};
