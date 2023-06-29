// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
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

	UFUNCTION(BlueprintCallable)
	void CreateServer(bool ShouldCounsel);

	UFUNCTION(BlueprintCallable)
	void FindServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer(int Index);

	UFUNCTION(BlueprintCallable)
	void CloseServer();

private:
	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnDestroySessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionsComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	TSubclassOf<class UUserWidget> MainWidgetClass;
	class UMainWidget* MainWidget;

	UFUNCTION(BlueprintCallable)
	void LoadMainWidget();

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;
	TArray<FOnlineSessionSearchResult> FoundResults;

	bool bIsCounselMode = false;
};
