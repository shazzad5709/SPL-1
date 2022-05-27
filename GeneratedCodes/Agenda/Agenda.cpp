#include "Agenda.h"
#include <algorithm>

const std::string& Agenda::getName() {
	std::lock_guard<std::mutex> lock(nameMutex);
	return name;
}

void Agenda::setName(const std::string& name_) {
	std::lock_guard<std::mutex> lock(nameMutex);
	name = name_;
}

std::vector<Contact * const> Agenda::getContacts() {
	std::lock_guard<std::mutex> lock(contactsMutex);

	std::vector<Contact * const> v;
	v.reserve(contacts.size());

	for(auto& e : contacts) {
		v.push_back(e.get());
	}

	return v;
}

bool Agenda::hasContact(const std::unique_ptr<Contact>& contact) {
	std::lock_guard<std::mutex> lock(contactsMutex);
	return std::find(contacts.begin(), contacts.end(), contact) != contacts.end();
}

bool Agenda::addContact(std::unique_ptr<Contact> contact) {
	std::lock_guard<std::mutex> lock(contactsMutex);
	auto it = std::find(contacts.begin(), contacts.end(), contact);
	if(it == contacts.end()) {
		contacts.push_back(contact);
		return true;
	}
	return false;
}

bool Agenda::removeContact(const std::unique_ptr<Contact>& contact) {
	std::lock_guard<std::mutex> lock(contactsMutex);
	auto it = std::find(contacts.begin(), contacts.end(), contact);
	if(it != contacts.end()) {
		contacts.erase(it);
		return true;
	}
	return false;
}

void Agenda::clearContacts() {
	std::lock_guard<std::mutex> lock(contactsMutex);
	contacts.clear();
}

const Contact& Agenda::getProfile()  {
	std::lock_guard<std::mutex> lock(profileMutex);
	return *profile;
}

void Agenda::setProfile(std::unique_ptr<Contact> profile_) {
	std::lock_guard<std::mutex> lock(profileMutex);
	profile = std::move(profile_);
}


Agenda::Agenda(const std::string& name_, std::unique_ptr<Contact>profile_, const std::vector<std::unique_ptr<Contact>>& contacts_)
:name(name_)
,profile(std::move(profile_))
,contacts(contacts_)
{}

void Agenda::showContacts()
{

}

