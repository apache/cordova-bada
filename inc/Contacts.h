/*
 * Contacts.h
 *
 *  Created on: Mar 25, 2011
 *      Author: Anis Kadri
 */

#ifndef CONTACTS_H_
#define CONTACTS_H_

#include <FApp.h>
#include "PhoneGapCommand.h"
using namespace Osp::Base::Collection;
using namespace Osp::App;

class Contacts: public PhoneGapCommand {
public:
	Contacts(Web* pWeb);
	virtual ~Contacts();
public:
	void Run(const String& command);
	void Create();
	void Find();
};

#endif /* CONTACTS_H_ */
