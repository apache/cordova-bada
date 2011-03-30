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

class Contacts: public PhoneGapCommand, IAppControlEventListener {
public:
	Contacts(Web* pWeb);
	virtual ~Contacts();
public:
	void Run(const String& command);
	void Create(const String& query);
	void Find();
public:
	virtual void OnAppControlCompleted (const Osp::Base::String &appControlId, const Osp::Base::String &operationId, const Osp::Base::Collection::IList *pResultList);
private:
	ArrayList* GetDataList(const String& query);
	String callbackId;
};

#endif /* CONTACTS_H_ */
