/*
 * ProxySensor.h
 *
 *  Created on: Nov 24, 2014
 *      Author: jowonbin
 */

#ifndef PROXYSENSOR_H_
#define PROXYSENSOR_H_

#include <FBase.h>
#include <FUix.h>
#include <FUi.h>
#include "SceneRegister.h"


using namespace Tizen::Base;
using namespace Tizen::Uix::Sensor;

class ProxySensor :
 	public Tizen::Uix::Sensor::ISensorEventListener{
public:
	ProxySensor();
	virtual ~ProxySensor();
	void 	OnDataReceived (SensorType sensorType, SensorData &sensorData, result r);

private:
	void OpenCalculator();

	bool isDetected;
	Tizen::Uix::Sensor::SensorManager __sensorMgr;
};

#endif /* PROXYSENSOR_H_ */
