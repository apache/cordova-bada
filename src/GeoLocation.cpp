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
	if(command == "startWatching") {

	}
	else if(command == "StopWatching") {

	} else if(command == "GetLastKnownLocation") {

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
	String* jsResponse = null;
	Location *location = locProvider->GetLastKnownLocationN();
	if(location->GetQualifiedCoordinates() != null) {
		const QualifiedCoordinates *q = location->GetQualifiedCoordinates();
		double latitude = q->GetLatitude();
		double longitude = q->GetLongitude();
		float altitude = q->GetAltitude();
		AppLogDebug("latitude %d longitude %d altitude %f", latitude, longitude, altitude);
		String res;
		res.Format(128, L"updateLocation(%d, %d, %f)", latitude, longitude, altitude);
		jsResponse = pWeb->EvaluateJavascriptN(res);
		AppLogDebug("Result: %S", jsResponse->GetPointer());
	} else {
		AppLogDebug("Could not get location");
		jsResponse = pWeb->EvaluateJavascriptN(L"locationErrorCallback('Could not get location');");
		AppLogDebug("Result: %S", jsResponse->GetPointer());
	}
	delete jsResponse;
}

void
GeoLocation::OnLocationUpdated(Location& location) {
	String* jsResponse = null;
	if(location.GetQualifiedCoordinates() != null) {
		AppLogDebug("GOT LOCATION");
		const QualifiedCoordinates *q = location.GetQualifiedCoordinates();
		double latitude = q->GetLatitude();
		double longitude = q->GetLongitude();
		float altitude = q->GetAltitude();
		float speed = location.GetSpeed();
		AppLogDebug("latitude %d longitude %d altitude %f speed %f", latitude, longitude, altitude, speed);
		String res;
		res.Format(128, L"updateLocation(%d, %d, %f, %f)", latitude, longitude, altitude, speed);
		jsResponse = pWeb->EvaluateJavascriptN(res);
		AppLogDebug("Result: %S", jsResponse->GetPointer());
	} else {
		AppLogDebug("Could not get location");
		jsResponse = pWeb->EvaluateJavascriptN(L"locationErrorCallback('Could not get location');");
		AppLogDebug("Result: %S", jsResponse->GetPointer());
	}
	delete jsResponse;
}

void
GeoLocation::OnProviderStateChanged(LocProviderState newState) {

}
