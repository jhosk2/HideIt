/*
 * ProxySensor.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: jowonbin
 */

#include "ProxySensor.h"

using namespace Tizen::Ui::Scenes;

ProxySensor::ProxySensor()
	: isDetected(false){
	// TODO Auto-generated constructor stub

	__sensorMgr.Construct();

	/* Check for the Sensors is supported in the current device or not */
	if (!__sensorMgr.IsAvailable(SENSOR_TYPE_PROXIMITY))
	{
		throw "ProxySensor Not Available";
		//return E_FAILURE;
	}

	long interval = 0L;

	__sensorMgr.GetMinInterval(SENSOR_TYPE_PROXIMITY, interval);
	if (interval < 500)
	{
		interval = 500;
	}

	__sensorMgr.AddSensorListener(*this, SENSOR_TYPE_PROXIMITY, interval, true);
	AppLog("Proxy Sensor Constructor");

}

ProxySensor::~ProxySensor() {
	// TODO Auto-generated destructor stub
	__sensorMgr.RemoveSensorListener(*this);
}

void ProxySensor::OnDataReceived (SensorType sensorType, SensorData &sensorData, result r)
{
	TryReturnVoid(!IsFailed(r), "[%s] Invalid sensor data.", GetErrorMessage(r));

	ProximitySensorData& data = static_cast<ProximitySensorData&>(sensorData);

	String ProximityDist;
	bool ObjDetected =	data.IsObjectDetected();

	if(ObjDetected==true)
	{
		if ( isDetected )
		{
			OpenCalculator();
		}
		else
		{
			isDetected = true;
		}
	}
	else
	{
		isDetected = false;
	}



}

void ProxySensor::OpenCalculator(){
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	//pSceneManager->GoBackward(BackwardSceneTransition(SCENE_CALCULATOR));
	AppLog("detected");
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_CALCULATOR));


}
