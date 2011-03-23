/*
 * Accelerometer.cpp
 *
 *  Created on: Mar 8, 2011
 *      Author: Anis Kadri
 */

#include "Accelerometer.h"

Accelerometer::Accelerometer() {
	__sensorMgr.Construct();
	started = false;
}

Accelerometer::Accelerometer(Web* pWeb, ArrayList* settings): PhoneGapCommand(pWeb, settings) {
	__sensorMgr.Construct();
	started = false;
	x = y = z = 0.0;
	timestamp = 0;
}

Accelerometer::~Accelerometer() {
}

void
Accelerometer::Run(const String& command) {
	if (!command.IsEmpty()) {

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
		if(method == L"com.phonegap.Accelerometer.watchAcceleration" && !uuid.IsEmpty() && !IsStarted()) {
			AppLogDebug("watching acceleration...");
			StartSensor();
		}
		if(method == L"com.phonegap.Accelerometer.clearWatch" && !uuid.IsEmpty() && IsStarted()) {
			AppLogDebug("stop watching acceleration...");
			StopSensor();
		}
		if(method == L"com.phonegap.Accelerometer.getCurrentAcceleration" && !uuid.IsEmpty() && !IsStarted()) {
			AppLogDebug("getting current acceleration...");
			GetLastAcceleration();
		}
		AppLogDebug("Acceleration command %S completed", command.GetPointer());
	} else {
		AppLogDebug("Can't run empty command");
	}
}

bool
Accelerometer::StartSensor(void) {
	result r = E_SUCCESS;

	if(__sensorMgr.IsAvailable(SENSOR_TYPE_ACCELERATION)) {
		r = __sensorMgr.AddSensorListener(*this, SENSOR_TYPE_ACCELERATION, 50, true);
		if(IsFailed(r)) {
			return false;
		}
	} else {
		AppLogException("Acceleration sensor is not available");
		String res;
		String* jsResponse;
		res.Format(256, L"navigator.accelerometer.fail({message:'Acceleration sensor is not available',code:'001'});");
		jsResponse = pWeb->EvaluateJavascriptN(res);
		AppLogDebug("Result: %S", jsResponse->GetPointer());
		delete jsResponse;
		return false;
	}
	started = true;
	AppLogDebug("Start Watching Sensor");
	return true;
}

bool
Accelerometer::StopSensor(void) {
	result r = E_SUCCESS;

	r = __sensorMgr.RemoveSensorListener(*this, SENSOR_TYPE_ACCELERATION);
	if(IsFailed(r)) {
		return false;
	}
	started = false;
	AppLogDebug("Stopped Watching Sensor");
	return true;
}

bool
Accelerometer::IsStarted() {
	return started;
}

void
Accelerometer::GetLastAcceleration() {
	String* jsResponse;
	String res;
	res.Format(256, L"navigator.accelerometer.success('%S', {x:%f,y:%f,z:%f,timestamp:%d});", uuid.GetPointer(), x, y, z, timestamp);
	jsResponse = pWeb->EvaluateJavascriptN(res);
	AppLogDebug("Result: %S", jsResponse->GetPointer());
	delete jsResponse;
}

void
Accelerometer::OnDataReceived(SensorType sensorType, SensorData& sensorData, result r) {
	String* jsResponse;

	sensorData.GetValue((SensorDataKey)ACCELERATION_DATA_KEY_TIMESTAMP, timestamp);
	sensorData.GetValue((SensorDataKey)ACCELERATION_DATA_KEY_X, x);
	sensorData.GetValue((SensorDataKey)ACCELERATION_DATA_KEY_Y, y);
	sensorData.GetValue((SensorDataKey)ACCELERATION_DATA_KEY_Z, z);

	AppLogDebug("x: %f, y: %f, z: %f timestamp: %d", x, y, z, timestamp);

	String res;
	res.Format(256, L"navigator.accelerometer.success('%S', {x:%f,y:%f,z:%f,timestamp:%d});", uuid.GetPointer(), x, y, z, timestamp);
	AppLogDebug("%S", res.GetPointer());
	jsResponse = pWeb->EvaluateJavascriptN(res);
	AppLogDebug("Result: %S", jsResponse->GetPointer());
	delete jsResponse;
}
