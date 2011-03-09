/*
 * Device.h
 *
 *  Created on: Mar 8, 2011
 *      Author: Anis Kadri
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "PhoneGapCommand.h"
#include "FSystem.h"

using namespace Osp::System;

class Device: public PhoneGapCommand {
public:
	Device();
	Device(Web* pWeb, ArrayList* settings);
	virtual ~Device();
public:
	result GetSystemInfo();
	virtual void Run(const String& command);
};

#endif /* DEVICE_H_ */
