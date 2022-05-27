#ifndef __PROFESSIONNAL_CONTACT_H__
#define __PROFESSIONNAL_CONTACT_H__
#include <string>
#include <vector>
#include <mutex>
#include <memory>

#include "ProfessionnalContact.h"
#include "Contact.h"

class ProfessionnalContact: public Contact
{
	std::vector<std::string> additionnalEmails;
	std::mutex additionnalEmailsMutex;
	std::vector<std::weak_ptr<ProfessionnalContact>> associates;
	std::mutex associatesMutex;

public: 
	const std::vector<std::string> getAdditionnalEmails();
	bool hasAdditionnalEmail(const std::string& additionnalEmail);
	bool addAdditionnalEmail(const std::string& additionnalEmail);
	bool removeAdditionnalEmail(const std::string& additionnalEmail);
	void clearAdditionnalEmails();

	const std::vector<std::weak_ptr<ProfessionnalContact>> getAssociates();
	bool hasAssociate(const std::weak_ptr<ProfessionnalContact> associate);
	bool addAssociate(const std::weak_ptr<ProfessionnalContact> associate);
	bool removeAssociate(const std::weak_ptr<ProfessionnalContact> associate);
	void clearAssociates();


	ProfessionnalContact(std::unique_ptr<Person>person_, const std::string& phoneNumber_, const std::string& email_, const std::vector<std::string>& additionnalEmails_);
	virtual ~ProfessionnalContact() {}

	virtual void showInfo();
};

#endif

