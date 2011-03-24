/*
 * GeoLocation.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: Anis Kadri
 */

#include "GeoLocation.h"

GeoLocation::GeoLocation() {
	// TODO Auto-generated constructor stub

}

GeoLocation::GeoLocation(Web* pWeb, ArrayList* settings): PhoneGapCommand(pWeb, settings) {
	locProvider = new LocationProvider();
	locProvider->Construct(LOC_METHOD_HYBRID);
	watching = false;
}

GeoLocation::~GeoLocation() {
	delete locProvider;
}

void
GeoLocation::Run(const String& command) {
	if(!command.IsEmpty()) {
		String args;
		String delim(L"/");
		command.SubString(String(L"gap://").GetLength(), args);
		StringTokenizer strTok(args, delim);
		String method;
		strTok.GetNextToken(method);
		// Getting UUID
		for(int i = 0 ; i < 2 && strTok.HasMoreTokens() ; i++, strTok.GetNextToken(uuid));
		AppLogDebug("Method %S, UUID: %S", method.GetPointer(), uuid.GetPointer());
		// used to determine callback ID
		if(method == L"com.phonegap.Geolocation.watchPosition" && !uuid.IsEmpty() && !IsWatching()) {
			AppLogDebug("watching position...");
			StartWatching();
		}
		if(method == L"com.phonegap.Geolocation.stop" && !uuid.IsEmpty() && IsWatching()) {
			AppLogDebug("stop watching position...");
			StopWatching();
		}
		if(method == L"com.phonegap.Geolocation.getCurrentPosition" && !uuid.IsEmpty() && !IsWatching()) {
			AppLogDebug("getting current position...");
			GetLastKnownLocation();
		}
		AppLogDebug("GeoLocation command %S completed", command.GetPointer());
	}
}

void
GeoLocation::StartWatching() {
	locProvider->RequestLocationUpdates(*this, 5, false);
	watching = true;
	AppLogDebug("Start Watching Location");
}

void
GeoLocation::StopWatching() {
	locProvider->CancelLocationUpdates();
	watching = false;
	AppLogDebug("Stop Watching Location");
}

bool
GeoLocation::IsWatching() {
	return watching;
}

void
GeoLocation::GetLastKnownLocation() {
	Location *location = locProvider->GetLastKnownLocationN();
	if(location->GetQualifiedCoordinates() != null) {
		const QualifiedCoordinates *q = location->GetQualifiedCoordinates();
		double latitude = q->GetLatitude();
		double longitude = q->GetLongitude();
		float altitude = q->GetAltitude();
		float accuracy = q->GetHorizontalAccuracy();
		float heading = q->GetVerticalAccuracy();
		float speed = location->GetSpeed();
		long long timestamp = location->GetTimestamp();
		AppLogDebug("{latitude:%d,longitude:%d,altitude:%f,speed:%f,accuracy:%f,heading:%f,timestamp:%d}", latitude, longitude, altitude,
																									  speed, accuracy, heading, timestamp);
		String pos;
		pos.Format(256, L"{latitude:%d,longitude:%d,altitude:%f,speed:%f,accuracy:%f,heading:%f,timestamp:%d}", latitude, longitude, altitude,
																										  speed, accuracy, heading, timestamp);
		String res;
		res.Format(512, L"navigator.geolocation.success('%S', {message:%S})", uuid.GetPointer(), pos.GetPointer());
		pWeb->EvaluateJavascriptN(res);
	} else {
		AppLogDebug("navigator.geolocation.fail('%S', {status: '001',message:'Could not get location'});", uuid.GetPointer());
		String res;
		res.Format(256, L"navigator.geolocation.fail('%S', {status: '001',message:'Could not get location'});", uuid.GetPointer());
		pWeb->EvaluateJavascriptN(res);
	}
}

void
GeoLocation::OnLocationUpdated(Location& location) {
	if(location.GetQualifiedCoordinates() != null) {
		const QualifiedCoordinates *q = location.GetQualifiedCoordinates();
		double latitude = q->GetLatitude();
		double longitude = q->GetLongitude();
		float altitude = q->GetAltitude();
		float accuracy = q->GetHorizontalAccuracy();
		float heading = q->GetVerticalAccuracy();
		float speed = location.GetSpeed();
		long long timestamp = location.GetTimestamp();
		AppLogDebug("{latitude:%d,longitude:%d,altitude:%f,speed:%f,accuracy:%f,heading:%f,timestamp:%d}", latitude, longitude, altitude,
																									  speed, accuracy, heading, timestamp);
		String pos;
		pos.Format(256, L"{latitude:%d,longitude:%d,altitude:%f,speed:%f,accuracy:%f,heading:%f,timestamp:%d}", latitude, longitude, altitude,
																										   speed, accuracy, heading, timestamp);
		String res;
		res.Format(512, L"navigator.geolocation.success('%S', {message:%S})", uuid.GetPointer(), pos.GetPointer());
		AppLogDebug("%S", res.GetPointer());
		pWeb->EvaluateJavascriptN(res);
	} else {
		AppLogDebug("navigator.geolocation.fail('%S', {status: '001',message:'Could not get location'});", uuid.GetPointer());
		String res;
		res.Format(256, L"navigator.geolocation.fail('%S', {status: '001',message:'Could not get location'});", uuid.GetPointer());
		pWeb->EvaluateJavascriptN(res);
	}
}

void
GeoLocation::OnProviderStateChanged(LocProviderState newState) {

}
