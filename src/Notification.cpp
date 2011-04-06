/*
 * Notification.cpp
 *
 *  Created on: Apr 5, 2011
 *      Author: Anis Kadri
 */

#include "../inc/Notification.h"

Notification::Notification(Web* pWeb) : PhoneGapCommand(pWeb) {
}

Notification::~Notification() {
}

void
Notification::Run(const String& command) {
	if(!command.IsEmpty()) {
		Uri commandUri;
		commandUri.SetUri(command);
		String method = commandUri.GetHost();
		StringTokenizer strTok(commandUri.GetPath(), L"/");
		if(strTok.GetTokenCount() < 1) {
			AppLogException("Not enough params");
			return;
		}
		strTok.GetNextToken(callbackId);
		AppLogDebug("%S %S", method.GetPointer(), callbackId.GetPointer());
		if((method == L"com.phonegap.Notification.alert" || method == L"com.phonegap.Notification.confirm") && !callbackId.IsEmpty()) {
			Dialog();
		} else if(method == L"com.phonegap.Notification.vibrate" && !callbackId.IsEmpty()) {
			//Vibrate();
		} else if(method == L"com.phonegap.Notification.beep" && !callbackId.IsEmpty()) {
			//Beep();
		}
	}
}

void
Notification::Dialog() {
	MessageBox messageBox;
	String* title;
	String* message;
	String* styleStr;
	String eval;

	title = pWeb->EvaluateJavascriptN(L"navigator.notification.messageBox.title");
	message = pWeb->EvaluateJavascriptN(L"navigator.notification.messageBox.message");
	styleStr = pWeb->EvaluateJavascriptN(L"navigator.notification.messageBox.messageBoxStyle");

	AppLogDebug("title %S message %S styleStr %S", title->GetPointer(), message->GetPointer(), styleStr->GetPointer());
	if(!title->IsEmpty() && !message->IsEmpty() && !styleStr->IsEmpty()) {
		int style;
		int modalResult = 0;
		if(Integer::Parse(*styleStr, style) != E_SUCCESS) {
			AppLogException("Could not get dialog style");
			return;
		}
		messageBox.Construct(*title, *message, (MessageBoxStyle)style, 0);
		messageBox.ShowAndWait(modalResult);
		switch(modalResult) {
		case MSGBOX_RESULT_CLOSE:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('Close')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_OK:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('OK')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_CANCEL:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('Cancel')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_YES:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('Yes')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_NO:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('No')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_ABORT:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('Abort')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_TRY:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('Try')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_RETRY:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('Retry')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_IGNORE:
			eval.Format(128, L"PhoneGap.callbacks['%S'].success('Ignore')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		case MSGBOX_RESULT_CONTINUE:
			eval.Format(64, L"PhoneGap.callbacks['%S'].success('Continue')", callbackId.GetPointer());
			pWeb->EvaluateJavascriptN(eval);
			break;
		}

	} else {
		AppLogException("Could not construct MessageBox");
	}
	delete title;
	delete message;
	delete styleStr;
}
void Notification::Vibrate() {
	Vibrator vibrator;
	vibrator.Construct();
	vibrator.Start(3000,   // on period
		           1000,  // off period
		           2,   // repeat count
		           60);   // vibration strength
}

void Notification::Beep() {

}
