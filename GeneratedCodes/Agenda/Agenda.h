#ifndef __AGENDA_H__
#define __AGENDA_H__
#include <string>
#include <vector>
#include <mutex>
#include <memory>

#include "Contacts/Contact.h"

class Agenda
{
	std::string name;
	std::mutex nameMutex;
	std::vector<std::unique_ptr<Contact>> contacts;
	std::mutex contactsMutex;
	std::unique_ptr<Contact> profile;
	std::mutex profileMutex;

public: 
	const std::string& getName();
	void setName(const std::string& name_);

	std::vector<Contact * const> getContacts();
private:
	bool hasContact(const Contact * const contact);
public:
	bool addContact(std::unique_ptr<Contact> contact);
	bool removeContact(const Contact * const contact);
	void clearContacts();

	const Contact& getProfile();
	void setProfile(std::unique_ptr<Contact> profile_);


	Agenda(const std::string& name_, std::unique_ptr<Contact>profile_, const std::vector<std::unique_ptr<Contact>>& contacts_);
	virtual ~Agenda() {}

	virtual void showContacts();
};

#endif

