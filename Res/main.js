function externalBrowser() {
	window.location.href = "http://www.phonegap.com";
}

function getLastKnownLocation() {
	window.location.href = "gap://GeoLocation.GetLastKnownLocation";
}

function watchPosition() {
	window.location.href = "gap://GeoLocation.StartWatching";
}

function clearWatch() {
	window.location.href = "gap://GeoLocation.StopWatching";
}

function getSystemInfo() {
	window.location.href = "gap://Device.GetSystemInfo";
}

function watchAcceleration() {
	window.location.href = "gap://Accelerometer.StartSensor";
}

function clearWatchAcceleration() {
	window.location.href = "gap://Accelerometer.StopSensor";
}

function toggleWatchPosition(em) {
	var coord = document.getElementById('location');
	if(em.value == "GeoLocation.StartWatching") {
		em.value = "GeoLocation.StopWatching";
		coord.style.display = 'block';
		watchPosition();
	} else {
		em.value = "GeoLocation.StartWatching";
		coord.style.display = 'none';
		clearWatch();
	}
}

function toggleStartSensor(em) {
	var accelerometer = document.getElementById('accelerometer');
	if(em.value == "Accelerometer.StartSensor") {
		em.value = "Accelerometer.StopSensor";
		accelerometer.style.display = 'block';
		watchAcceleration();
	} else {
		em.value = "Accelerometer.StartSensor";
		accelerometer.style.display = 'none';
		clearWatchAcceleration();
	}
}

function updateLocation(latitude, longitude, altitude, speed) {
	var location = document.getElementById('location');
	location.innerHTML = "Latitude: " + latitude + 
					  ", Longitude:  " + longitude + 
					  ", Altitude: " + altitude
					  ", Speed:  " + speed;
	return "updateCoordinates SUCCESS!";
}

function updateSensorData(acceleration) {
	var accel = document.getElementById('accelerometer');
	accel.innerHTML = "x: "+acceleration.x +
					  ", y: "+acceleration.y +
					  ", z: "+acceleration.z +
					  ", timeStamp: "+acceleration.timestamp;
	return "updateSensorData SUCCESS!";
}

function sensorErrorCallback(error) {
	var accel = document.getElementById('accelerometer');
	accel.innerHTML = error;
	return "sensorErrorCallback SUCCESS!"
}

function locationErrorCallback(error) {
	var coord = document.getElementById('location');
	coord.innerHTML = error;
	return "locationErrorCallback SUCCESS!";
}

function updateSystemInfo(info, error) {
	var system = document.getElementById('system');
	system.style.display = 'block';
	if(error) {
		system.innerHTML = error;
	} else {
		system.innerHTML = "Name: "+info.PlatformName+
							", PlatformVersion: "+info.PlatformVersion +
							", APIVersion: "+info.APIVersion;
	}
	return "updateSystemInfo SUCCESS!";
}