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
		// Saving a new contact
		if(method == L"com.phonegap.Contacts.save" && !callbackId.IsEmpty()) {
			String contactId;
			strTok.GetNextToken(contactId);
			int cid = -1;
			result r = E_SUCCESS;
			r = Integer::Parse(contactId, cid);
			if(IsFailed(r)) {
				AppLogException("Could not retrieve contact ID");
			}
			AppLogDebug("Method %S callbackId %S contactId %d", method.GetPointer(), callbackId.GetPointer(), cid);
			Create(cid);
		// Finding an exisiting contact by Name/Phone Number/Email
		} else if(method == L"com.phonegap.Contacts.find" && !callbackId.IsEmpty()) {
			String filter;
			strTok.GetNextToken(filter);
			AppLogDebug("Method %S callbackId %S filter %S", method.GetPointer(), callbackId.GetPointer(), filter.GetPointer());
			Find(filter);
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

			if(!type->IsEmpty() && !number->IsEmpty()) {
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
	// Getting emails length
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

			// Getting email type
			eval.Clear();
			eval.Format(64, L"navigator.service.contacts.records[%d].emails[%d].type", cid, i);
			type = pWeb->EvaluateJavascriptN(eval);

			// Getting email
			eval.Clear();
			eval.Format(64, L"navigator.service.contacts.records[%d].emails[%d].value", cid, i);
			address = pWeb->EvaluateJavascriptN(eval);

			if(!type->IsEmpty() && !address->IsEmpty()) {
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
	// Getting emails length
	String* lengthStr = NULL;
	String eval;
	eval.Format(64, L"navigator.service.contacts.records[%d].urls.length", cid);
	lengthStr = pWeb->EvaluateJavascriptN(eval);
	if(!lengthStr->IsEmpty()) {
		int length;
		result r = Integer::Parse(*lengthStr, length);
		if(IsFailed(r)) {
			AppLogException("Could not get urls length");
			return;
		}
		delete lengthStr;
		for(int i = 0 ; i < length ; i++) {
			String* type = NULL;
			String* address = NULL;

			// Getting url type
			eval.Clear();
			eval.Format(64, L"navigator.service.contacts.records[%d].urls[%d].type", cid, i);
			type = pWeb->EvaluateJavascriptN(eval);

			// Getting url
			eval.Clear();
			eval.Format(64, L"navigator.service.contacts.records[%d].urls[%d].value", cid, i);
			address = pWeb->EvaluateJavascriptN(eval);

			if(!type->IsEmpty() && !address->IsEmpty()) {
				if(*type == URL_TYPE_PERSONAL) {
			        Url url(URL_TYPE_PERSONAL, *address);
			        contact.AddUrl(url);
				} else if(*type == URL_TYPE_WORK) {
			        Url url(URL_TYPE_WORK, *address);
			        contact.AddUrl(url);
				} else if(*type == URL_TYPE_OTHER) {
			        Url url(URL_TYPE_OTHER, *address);
			        contact.AddUrl(url);
				}
			}
			delete type;
			delete address;
		}
	}
}

void
Contacts::SetOrganization(Contact& contact, const int cid) {
	// Setting Organization Name
	String* value = NULL;
	String eval;
	eval.Format(64, L"navigator.service.contacts.records[%d].organization.name", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("Organization Name: %S", value->GetPointer());
		contact.SetValue(CONTACT_PROPERTY_ID_COMPANY, *value);
	}
	delete value;

	// Setting Organization Title
	eval.Clear();
	eval.Format(64, L"navigator.service.contacts.records[%d].organization.title", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("Organization Title: %S", value->GetPointer());
		contact.SetValue(CONTACT_PROPERTY_ID_JOB_TITLE, *value);
	}
	delete value;
}
void
Contacts::SetBirthday(Contact& contact, const int cid) {
	String* value;
	String eval;
	int year, month, day;
	DateTime birthday;

	// Setting Year
	eval.Format(64, L"navigator.service.contacts.records[%d].birthday.getFullYear()", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		result r = Integer::Parse(*value, year);
		if(IsFailed(r)) {
			AppLogException("Could not get birthday Year");
			return;
		}
		AppLogDebug("Birthday Year: %S", value->GetPointer());
	}
	delete value;

	// Setting Month
	eval.Clear();
	eval.Format(64, L"navigator.service.contacts.records[%d].birthday.getMonth() + 1", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		result r = Integer::Parse(*value, month);
		if(IsFailed(r)) {
			AppLogException("Could not get birthday Month");
			return;
		}
		AppLogDebug("Birthday Month: %S", value->GetPointer());
	}
	delete value;

	// Setting Day
	eval.Clear();
	eval.Format(64, L"navigator.service.contacts.records[%d].birthday.getDate()", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		result r = Integer::Parse(*value, day);
		if(IsFailed(r)) {
			AppLogException("Could not get birthday Day");
			return;
		}
		AppLogDebug("Birthday Day: %S", value->GetPointer());
	}
	delete value;

	birthday.SetValue(year, month, day);
	contact.SetValue(CONTACT_PROPERTY_ID_BIRTHDAY, birthday);
	AppLogDebug("Birthday %d/%d/%d added", year, month, day);
}

void
Contacts::SetAddress(Contact& contact, const int cid) {
	Address address;
	String* value;
	String eval;
	// Setting Street Address
	eval.Format(64, L"navigator.service.contacts.records[%d].address.streetAddress", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("Street Address: %S", value->GetPointer());
		address.SetStreet(*value);
	}
	delete value;

	// Setting City
	eval.Format(64, L"navigator.service.contacts.records[%d].address.locality", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("City: %S", value->GetPointer());
		address.SetCity(*value);
	}
	delete value;

	// Setting State
	eval.Format(64, L"navigator.service.contacts.records[%d].address.region", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("State: %S", value->GetPointer());
		address.SetState(*value);
	}
	delete value;

	// Setting Postal Code
	eval.Format(64, L"navigator.service.contacts.records[%d].address.postalCode", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("Postal Code: %S", value->GetPointer());
		address.SetPostalCode(*value);
	}
	delete value;

	// Setting Country
	eval.Format(64, L"navigator.service.contacts.records[%d].address.country", cid);
	value = pWeb->EvaluateJavascriptN(eval);
	if(!value->IsEmpty()) {
		AppLogDebug("County: %S", value->GetPointer());
		address.SetPostalCode(*value);
	}
	delete value;

	contact.AddAddress(address);
	AppLogDebug("Address Added");
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

	String eval;

	if(IsFailed(r)) {
		AppLogException("Could not add contact");
		eval.Format(64, L"PhoneGap.callbacks['%S'].fail({message:'%s',code:%d})", callbackId.GetPointer(), r, GetErrorMessage(r));
		pWeb->EvaluateJavascriptN(eval);
	} else {
		AppLogDebug("Contact Successfully Added");
		eval.Format(64, L"PhoneGap.callbacks['%S'].success({message:'Contact added successfully'})", callbackId.GetPointer());
		pWeb->EvaluateJavascriptN(eval);
	}
}

void
Contacts::Find(const String& filter) {
	Addressbook addressbook;
	IList* pContactList = null;
	String jsContactObj("{displayName:%S,phoneNumber:%S,email:%S}");

	// Searching by Email
	pContactList = addressbook.SearchContactsByEmailN(filter);
	pContactList->RemoveAll(true);
	delete pContactList;

	// Searching by Name
	pContactList = addressbook.SearchContactsByNameN(filter);
	pContactList->RemoveAll(true);
	delete pContactList;

	// Searching by PhoneNumber
	pContactList = addressbook.SearchContactsByPhoneNumberN(filter);
	pContactList->RemoveAll(true);
	delete pContactList;
}
