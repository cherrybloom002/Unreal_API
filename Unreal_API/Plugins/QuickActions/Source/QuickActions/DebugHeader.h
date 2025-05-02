#pragma once

#include "QuickActions.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

void ScreenPrint(const FString& msg, const FColor& clr)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.f, clr, msg);
	}
}

void LogPrint(const FString& msg)
{
	UE_LOG(LogQuickActions, Log, TEXT("%s"), *msg);
}

EAppReturnType::Type ShowDialog(EAppMsgType::Type msgType,const FString & msg, bool bisWarning = true)
{
	if (bisWarning)
	{
		FText MsgTitle = FText::FromString(TEXT("Warning"));
		return FMessageDialog::Open(msgType, FText::FromString(msg), MsgTitle);
	}
	
	return FMessageDialog::Open(msgType, FText::FromString(msg));
}

void ShowNotifyInfo(const FString& msg)
{
	FNotificationInfo NotifyInfo(FText::FromString(msg));
	NotifyInfo.bUseLargeFont = true;
	NotifyInfo.FadeOutDuration = 5.f;
	FSlateNotificationManager::Get().AddNotification(NotifyInfo);
}