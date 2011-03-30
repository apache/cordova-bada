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
		// URL decoding
		Uri commandUri;
		commandUri.SetUri(command);
		String method = commandUri.GetHost();
		callbackId = commandUri.GetPath();
		callbackId.Replace(L"/", L"");
		String contactInfo = commandUri.GetQuery();

//		AppLogDebug("Method %S, callbackId %S, hostAddr %S URI %S", method.GetPointer(), callbackId.GetPointer(), hostAddr.GetPointer(), uri.ToString().GetPointer());
		AppLogDebug("Method %S callbackId %S query %S", method.GetPointer(), callbackId.GetPointer() , contactInfo.GetPointer());
		if(method == L"com.phonegap.Contacts.save" && !callbackId.IsEmpty()) {
			Create(contactInfo);
		}

	}
}

ArrayList*
Contacts::GetDataList(const String& query) {
	ArrayList* pDataList = new ArrayList();
	pDataList->Construct();
	String* pStorage = new String(L"storageType:phone");
	pDataList->Add(*pStorage);

	String delim(L"&");
	StringTokenizer strTok(query, delim);

	while(strTok.HasMoreTokens()) {
		String* pData = new String();
		strTok.GetNextToken(*pData);
		pData->Replace(L"=", L":");
		AppLogDebug("param %S", pData->GetPointer());
		pDataList->Add(*pData);
	}

	return pDataList;
}

void
Contacts::OnAppControlCompleted(const Osp::Base::String &appControlId,
								const Osp::Base::String &operationId,
								const Osp::Base::Collection::IList *pResultList) {
	AppLogDebug("appControlId %S operationId %S", appControlId.GetPointer(), operationId.GetPointer());
	for(int i = 0 ; i < pResultList->GetCount() ; i++) {
		String* obj = (String*)pResultList->GetAt(i);
		AppLogDebug("OBJ %S", obj->GetPointer());
	}
}

void
Contacts::Create(const String& query) {
	ArrayList* pDataList = GetDataList(query);
	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_CONTACT,OPERATION_ADD);
	if(pAc)
	{
		pAc->Start(pDataList, this);
		delete pAc;
	}
	pDataList->RemoveAll(true);
	delete pDataList;
}

void
Contacts::Find() {

}
