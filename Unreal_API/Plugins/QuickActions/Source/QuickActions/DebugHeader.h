#pragma once

#include "QuickActions.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

namespace DebugHeader 
{

	static void ScreenPrint(const FString& msg, const FColor& clr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.f, clr, msg);
		}
	}

	static void LogPrint(const FString& msg)
	{
		UE_LOG(LogQuickActions, Log, TEXT("%s"), *msg);
	}

	static EAppReturnType::Type ShowDialog(EAppMsgType::Type msgType,const FString & msg, bool bisWarning = true)
	{
		if (bisWarning)
		{
			FText MsgTitle = FText::FromString(TEXT("Warning"));
			return FMessageDialog::Open(msgType, FText::FromString(msg), MsgTitle);
		}
	
		return FMessageDialog::Open(msgType, FText::FromString(msg));
	}

	static void ShowNotifyInfo(const FString& msg)
	{
		FNotificationInfo NotifyInfo(FText::FromString(msg));
		NotifyInfo.bUseLargeFont = true;
		NotifyInfo.FadeOutDuration = 5.f;
		FSlateNotificationManager::Get().AddNotification(NotifyInfo);
	}
}