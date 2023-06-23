// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WooJooGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WOOJOO_API UWooJooGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWooJooGameInstance();

	UPROPERTY(BlueprintReadOnly)
	bool bUseLAN;

private:
	virtual void Init();

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;
	TArray<FOnlineSessionSearchResult> FoundResults;

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnDestroySessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionsComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void FindServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer();
};
