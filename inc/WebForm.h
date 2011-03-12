#ifndef _WEBFORM_H_
#define _WEBFORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FUi.h>
#include <FWeb.h>
#include <FSystem.h>
#include "PhoneGapCommand.h"
#include "GeoLocation.h"
#include "Device.h"
#include "Accelerometer.h"

using namespace Osp::Web::Controls;
using namespace Osp::Base;

class WebForm :
	public Osp::Ui::Controls::Form,
	public Osp::Ui::IActionEventListener,
	public Osp::Web::Controls::ILoadingListener
{

// Construction
public:
	WebForm(void);
	virtual ~WebForm(void);
	bool Initialize(void);

// Implementation
private:
	result CreateWebControl(void);

	Osp::Web::Controls::Web*	__pWeb;
	GeoLocation*                geolocation;
	Device*						device;
	Accelerometer*              accel;
	String*						__phonegapCommand;

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

public:
	virtual void  OnEstimatedProgress (int progress) {};
	virtual void  OnHttpAuthenticationCanceled (void) {};
	virtual bool  OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication) { return false; };
	virtual void  OnLoadingCanceled (void) {};
	virtual void  OnLoadingCompleted (void);
	virtual void  OnLoadingErrorOccurred (LoadingErrorType error, const Osp::Base::String &reason) {};
	virtual bool  OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type);
	virtual void  OnLoadingStarted (void) {};
	virtual void  OnPageTitleReceived (const Osp::Base::String &title) {};
	virtual DecisionPolicy  OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader) { return WEB_DECISION_CONTINUE; };

	virtual void LaunchBrowser(const String& url);

};

#endif	//_WebForm_H_
