/*
 * Device.cpp
 *
 *  Created on: Mar 8, 2011
 *      Author: Anis Kadri
 */

#include "../inc/Device.h"

Device::Device() {
	// TODO Auto-generated constructor stub

}

Device::Device(Web* pWeb, ArrayList* settings): PhoneGapCommand(pWeb, settings) {

}

Device::~Device() {
	// TODO Auto-generated destructor stub
}

void
Device::Run(const String& command) {

}

result
Device::GetSystemInfo() {
	result r = E_SUCCESS;
	String* jsResponse = null;
	String platformVersion;
	String apiVersion;
	int screen_height = 0;
	int screen_width = 0;

	/*screen*/
    r = SystemInfo::GetValue("ScreenWidth", screen_width);
    TryCatch(r == E_SUCCESS, , "SystemInfo: To get a value is failed");

    r = SystemInfo::GetValue("ScreenHeight", screen_height);
    TryCatch(r == E_SUCCESS, , "SystemInfo: To get a value is failed");

    r = SystemInfo::GetValue("PlatformVersion", platformVersion);
    TryCatch(r == E_SUCCESS, , "SystemInfo: To get a value is failed");

    r = SystemInfo::GetValue("APIVersion", apiVersion);
    TryCatch(r == E_SUCCESS, , "SystemInfo: To get a value is failed");

    if(r == E_SUCCESS) {
    	String res;
    	res.Format(128, L"updateSystemInfo({PlatformName: 'Bada', PlatformVersion:'%S', APIVersion:'%S'}, '')", platformVersion.GetPointer(),
																			     apiVersion.GetPointer());
    	jsResponse = pWeb->EvaluateJavascriptN(res);
    	AppLogDebug("%S", res.GetPointer());
    	AppLogDebug("Result: %S", jsResponse->GetPointer());
    	delete jsResponse;
    }
    return r;

CATCH:
	AppLog("Error = %s\n", GetErrorMessage(r));
    return r;
}
