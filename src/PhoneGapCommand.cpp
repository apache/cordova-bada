/*
 * PhoneGapCommand.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: Anis Kadri
 */

#include "PhoneGapCommand.h"

PhoneGapCommand::PhoneGapCommand() : pWeb(null), settings(null) {
}
PhoneGapCommand::PhoneGapCommand(Web* pWeb, ArrayList* settings) : pWeb(pWeb), settings(settings) {
}

PhoneGapCommand::~PhoneGapCommand() {
	delete pWeb;
	delete settings;
}
