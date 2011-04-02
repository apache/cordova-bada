/*
 * Contacts.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: Anis Kadri
 */

#include "../inc/Contacts.h"

Contacts::Contacts(Web* pWeb) : PhoneGapCommand(pWeb) {

}

Contacts::~Contacts() {
}

void
Contacts::Run(const String& command) {
	if(!command.IsEmpty()) {
		Uri commandUri;
		commandUri.SetUri(command);
		String method = commandUri.GetHost();
		StringTokenizer strTok(commandUri.GetPath(), L"/");
		if(strTok.GetTokenCount() < 2) {
			AppLogException("Not enough params");
			return;
		}
		strTok.GetNextToken(callbackId);
		String contactId;
		strTok.GetNextToken(contactId);
		int cid = -1;
		result r = E_SUCCESS;
		r = Integer::Parse(contactId, cid);
		if(IsFailed(r)) {
			AppLogException("Could not retrieve contact ID");
		}
		AppLogDebug("Method %S callbackId %S contactId %d", method.GetPointer(), callbackId.GetPointer(), cid);
		if(method == L"com.phonegap.Contacts.save" && !callbackId.IsEmpty()) {
			AppLogDebug("Saving contact...");
			Create(cid);
		}

	}
}

void
Contacts::SetNickname(Contact& contact, const int cid) {
	String* value = NULL;
	String eval;
	eval.Format(64, L"navigator.service.contacts.records[%d].nickname", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("nickname: %S", value->GetPointer());
		contact.SetValue(CONTACT_PROPERTY_ID_NICK_NAME, *value);
	}
	delete value;
}

void
Contacts::SetFirstName(Contact& contact, const int cid) {
	String* value = NULL;
	String eval;
	eval.Format(64, L"navigator.service.contacts.records[%d].name.givenName", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("First Name: %S", value->GetPointer());
		contact.SetValue(CONTACT_PROPERTY_ID_FIRST_NAME, *value);
	}
	delete value;
}

void
Contacts::SetLastName(Contact& contact, const int cid) {
	String* value = NULL;
	String eval;
	eval.Format(64, L"navigator.service.contacts.records[%d].name.familyName", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(value->IsEmpty()) {
		AppLogDebug("Last Name: %S", value->GetPointer());
		contact.SetValue(CONTACT_PROPERTY_ID_LAST_NAME, *value);
	}
	delete value;
}

void
Contacts::SetPhoneNumbers(Contact& contact, const int cid) {
	// Getting phone numbers length
	String* lengthStr = NULL;
	String eval;
	eval.Format(64, L"navigator.service.contacts.records[%d].phoneNumbers.length", cid);
	lengthStr = pWeb->EvaluateJavascriptN(eval);
	if(!lengthStr->IsEmpty()) {
		int length;
		result r = Integer::Parse(*lengthStr, length);
		if(IsFailed(r)) {
			AppLogException("Could not get phoneNumbers length");
			return;
		}
		delete lengthStr;
		for(int i = 0 ; i < length ; i++) {
			String* type = NULL;
			String* number = NULL;

			// Getting phone number type
			eval.Clear();
			eval.Format(64, L"navigator.service.contacts.records[%d].phoneNumbers[%d].type", cid, i);
			type = pWeb->EvaluateJavascriptN(eval);

			// Getting phone number
			eval.Clear();
			eval.Format(64, L"navigator.service.contacts.records[%d].phoneNumbers[%d].value", cid, i);
			number = pWeb->EvaluateJavascriptN(eval);

			if(type != NULL && number != NULL) {
				if(*type == PHONENUMBER_TYPE_HOME) {
					PhoneNumber phoneNumber(PHONENUMBER_TYPE_HOME, *number);
					contact.AddPhoneNumber(phoneNumber);
				} else if(*type == PHONENUMBER_TYPE_MOBILE) {
					PhoneNumber phoneNumber(PHONENUMBER_TYPE_MOBILE, *number);
					contact.AddPhoneNumber(phoneNumber);
				} else if(*type == PHONENUMBER_TYPE_PAGER) {
					PhoneNumber phoneNumber(PHONENUMBER_TYPE_PAGER, *number);
					contact.AddPhoneNumber(phoneNumber);
				} else if(*type == PHONENUMBER_TYPE_WORK) {
					PhoneNumber phoneNumber(PHONENUMBER_TYPE_WORK, *number);
					contact.AddPhoneNumber(phoneNumber);
				} else if(*type == PHONENUMBER_TYPE_OTHER) {
					PhoneNumber phoneNumber(PHONENUMBER_TYPE_OTHER, *number);
					contact.AddPhoneNumber(phoneNumber);
				}
			}
			delete type;
			delete number;
		}
	}
}

void
Contacts::SetEmails(Contact& contact, const int cid) {
	// Getting phone numbers length
	String* lengthStr = NULL;
	String eval;
	eval.Format(64, L"navigator.service.contacts.records[%d].emails.length", cid);
	lengthStr = pWeb->EvaluateJavascriptN(eval);
	if(!lengthStr->IsEmpty()) {
		int length;
		result r = Integer::Parse(*lengthStr, length);
		if(IsFailed(r)) {
			AppLogException("Could not get emails length");
			return;
		}
		delete lengthStr;
		for(int i = 0 ; i < length ; i++) {
			String* type = NULL;
			String* address = NULL;

			// Getting phone number type
			eval.Clear();
			eval.Format(64, L"navigator.service.contacts.records[%d].emails[%d].type", cid, i);
			type = pWeb->EvaluateJavascriptN(eval);

			// Getting phone number
			eval.Clear();
			eval.Format(64, L"navigator.service.contacts.records[%d].emails[%d].value", cid, i);
			address = pWeb->EvaluateJavascriptN(eval);

			if(type != NULL && address != NULL) {
				if(*type == EMAIL_TYPE_PERSONAL) {
			        Email email(EMAIL_TYPE_PERSONAL, *address);
			        contact.AddEmail(email);
				} else if(*type == EMAIL_TYPE_WORK) {
			        Email email(EMAIL_TYPE_WORK, *address);
			        contact.AddEmail(email);
				} else if(*type == EMAIL_TYPE_OTHER) {
			        Email email(EMAIL_TYPE_OTHER, *address);
			        contact.AddEmail(email);
				}
			}
			delete type;
			delete address;
		}
	}
}

void
Contacts::SetUrls(Contact& contact, const int cid) {
}

void
Contacts::SetOrganization(Contact& contact, const int cid) {

}
void
Contacts::SetBirthday(Contact& contact, const int cid) {
}

void
Contacts::SetAddress(Contact& contact, const int cid) {
}

void
Contacts::Create(const int cid) {
	result r = E_SUCCESS;
	Addressbook addressbook;

	r = addressbook.Construct();

	if(IsFailed(r)) {
		AppLogException("Could not create AddressBook");
		return;
	}

	Contact contact;
	SetNickname(contact, cid);
	SetFirstName(contact, cid);
	SetLastName(contact, cid);
	SetPhoneNumbers(contact, cid);
	SetEmails(contact, cid);
	SetUrls(contact, cid);
	SetOrganization(contact, cid);
	SetBirthday(contact, cid);
	SetAddress(contact, cid);

	r = addressbook.AddContact(contact);

	if(IsFailed(r)) {
		AppLogException("Could not add contact");
	} else {
		AppLogDebug("Contact Successfully Added");
	}
}

void
Contacts::Find() {

}
