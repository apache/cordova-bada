#include "WebForm.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::System;
using namespace Osp::Graphics;
using namespace Osp::Web::Controls;

WebForm::WebForm(void)
	:__pWeb(null), __phonegapCommand(null)
{
	location = null;
	device = null;
	accel = null;
}

WebForm::~WebForm(void)
{
	delete __phonegapCommand;
	delete location;
	delete device;
	delete accel;
}

bool
WebForm::Initialize()
{
	return true;
}

result
WebForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here

	r = CreateWebControl();
	if (IsFailed(r))
	{
		AppLog("CreateMainForm() has failed.\n");
		goto CATCH;
	}

	__pWeb->LoadUrl("file:///Res/index.html");

	return r;

CATCH:
	return false;
}

result
WebForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your termination code here

	return r;
}

void
WebForm::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		break;
	}
}

void
WebForm::LaunchBrowser(const String& url) {
	ArrayList* pDataList = null;
	pDataList = new ArrayList();
	pDataList->Construct();

	String* pData = null;
	pData = new String(L"url:");
	pData->Append(url);
	AppLogDebug("Launching Stock Browser with %S", pData->GetPointer());
	pDataList->Add(*pData);

	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_BROWSER, "");
	if(pAc) {
		pAc->Start(pDataList, null);
		delete pAc;
	}
	pDataList->RemoveAll(true);
	delete pDataList;
}

bool
WebForm::OnLoadingRequested (const Osp::Base::String& url, WebNavigationType type) {
	AppLogDebug("URL REQUESTED %S", url.GetPointer());
	if(url.StartsWith("gap://", 0)) {
		AppLogDebug("PhoneGap command");
		__phonegapCommand = new String(url);
		// FIXME: for some reason this does not work if we return true. Web freezes.
//		__pWeb->StopLoading();
//		String* test;
//		test = __pWeb->EvaluateJavascriptN(L"'test'");
//		AppLogDebug("String is %S", test->GetPointer());
//		delete test;
//		return true;
		return false;
	} else if(url.StartsWith("http://", 0) || url.StartsWith("https://", 0)) {
		AppLogDebug("Non PhoneGap command. External URL. Launching WebBrowser");
		LaunchBrowser(url);
		return false;
	} else if(url.StartsWith("file:///", 0)) {
		return false;
	}

	return false;
}

void
WebForm::OnLoadingCompleted() {
	int index;
	if(__phonegapCommand) {
		if(__phonegapCommand->IndexOf(L"GeoLocation", 0, index) == E_SUCCESS) {
			if(!location->IsWatching()) {
				location->StartWatching();
			} else {
				location->StopWatching();
			}
			delete __phonegapCommand;
			__phonegapCommand = null;
			AppLogDebug("GeoLocation command completed");
		}
		else if(__phonegapCommand->IndexOf(L"Device", 0, index) == E_SUCCESS) {
			device->GetSystemInfo();
			delete __phonegapCommand;
			__phonegapCommand = null;
			AppLogDebug("Device command completed");
		}
		else if(__phonegapCommand->IndexOf(L"Accelerometer", 0, index) == E_SUCCESS) {
			if(!accel->IsStarted()) {
				accel->StartSensor();
			} else {
				accel->StopSensor();
			}
			delete __phonegapCommand;
			__phonegapCommand = null;
			AppLogDebug("Accelerometer command completed");
		}
	}
	else {
		AppLogDebug("Non PhoneGap command completed");
	}
}

result
WebForm::CreateWebControl(void)
{
	result r = E_SUCCESS;
	int screen_width = 0;
	int screen_height = 0;

	/*screen*/
    r = SystemInfo::GetValue("ScreenWidth", screen_width);
    TryCatch(r == E_SUCCESS, , "SystemInfo: To get a value is failed");

    r = SystemInfo::GetValue("ScreenHeight", screen_height);
    TryCatch(r == E_SUCCESS, , "SystemInfo: To get a value is failed");

	/*Web*/
	__pWeb = new Web();
	r = __pWeb->Construct(Rectangle(0, 0, screen_width, screen_height - 38));
	TryCatch(r == E_SUCCESS, ,"Web is not constructed\n ");

	r = this->AddControl(*__pWeb);
	TryCatch(r == E_SUCCESS, ,"Web is not attached\n ");

	__pWeb->SetLoadingListener(this);

	__pWeb->SetFocus();

	if(__pWeb) {
		location = new GeoLocation(__pWeb, NULL);
		device = new Device(__pWeb, null);
		accel = new Accelerometer(__pWeb, null);
	}
	return r;

CATCH:
	AppLog("Error = %s\n", GetErrorMessage(r));
	return r;
}

