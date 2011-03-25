/*
 * Contacts.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: Anis Kadri
 */

#include "../inc/Contacts.h"

Contacts::Contacts(Web* pWeb) : PhoneGapCommand(pWeb) {
	// TODO Auto-generated constructor stub

}

Contacts::~Contacts() {
	// TODO Auto-generated destructor stub
}

void
Contacts::Run(const String& command) {
	if(!command.IsEmpty()) {

	}
}

void
Contacts::Create() {
	ArrayList* pDataList = null;
	pDataList = new ArrayList();
	pDataList->Construct();
	String* pData = null;
	pData = new String(L"phone:+919899045670");
	pDataList->Add(*pData);

	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_CONTACT,OPERATION_ADD);
	if(pAc)
	{
		pAc->Start(pDataList, null);
		delete pAc;
	}
	pDataList->RemoveAll(true);
	delete pDataList;
}

void
Contacts::Find() {

}
