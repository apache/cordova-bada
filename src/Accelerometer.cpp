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
}

Accelerometer::~Accelerometer() {
}

void
Accelerometer::Run(const String& command) {
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
Accelerometer::OnDataReceived(SensorType sensorType, SensorData& sensorData, result r) {
	long timeStamp = 0;
	float x = 0.0, y = 0.0, z = 0.0;
	String* jsResponse;

	sensorData.GetValue((SensorDataKey)ACCELERATION_DATA_KEY_TIMESTAMP, timeStamp);
	sensorData.GetValue((SensorDataKey)ACCELERATION_DATA_KEY_X, x);
	sensorData.GetValue((SensorDataKey)ACCELERATION_DATA_KEY_Y, y);
	sensorData.GetValue((SensorDataKey)ACCELERATION_DATA_KEY_Z, z);

	AppLogDebug("x: %f, y: %f, z: %f timestamp: %d", x, y, z, timeStamp);

	String res;
	res.Format(256, L"updateSensorData({x:%f, y:%f, z:%f, timestamp:%d})", x, y, z, timeStamp);
	jsResponse = pWeb->EvaluateJavascriptN(res);
	AppLogDebug("Result: %S", jsResponse->GetPointer());
	delete jsResponse;
}
