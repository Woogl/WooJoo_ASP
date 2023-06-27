// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class WOOJOO_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainWidget(const FObjectInitializer& ObjectInitializer);

	void SetLoadingScreen();

	void SetServerList(TArray<FOnlineSessionSearchResult> SessionSearchResults);

	void CantFindSession();

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> SearchingWidgetClass;
	class UUserWidget* SearchingWidget;

	TSubclassOf<class UUserWidget> NoSessionWidgetClass;
	class UUserWidget* NoSessionWidget;

	TSubclassOf<class UServerEntry> ServerEntryClass;
	class UUserWidget* ServerEntryWidget;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_SinglePlay;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_MultiPlay;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Counsel;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Quit;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Refresh;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Back;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	UFUNCTION()
	void OnClickedSinglePlay();
	UFUNCTION()
	void OnClickedMultiPlay();
	UFUNCTION()
	void OnClickedCounsel();
	UFUNCTION()
	void OnClickedQuit();
	UFUNCTION()
	void OnClickedHost();
	UFUNCTION()
	void OnClickedRefresh();
	UFUNCTION()
	void OnClickedBack();
};
