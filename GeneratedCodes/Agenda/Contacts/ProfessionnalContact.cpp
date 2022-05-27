#include "ProfessionnalContact.h"
#include <algorithm>

const std::vector<std::string> ProfessionnalContact::getAdditionnalEmails() {
	std::lock_guard<std::mutex> lock(additionnalEmailsMutex);
	return additionnalEmails;
}

bool ProfessionnalContact::hasAdditionnalEmail(const std::string& additionnalEmail) {
	std::lock_guard<std::mutex> lock(additionnalEmailsMutex);
	return std::find(additionnalEmails.begin(), additionnalEmails.end(), additionnalEmail) != additionnalEmails.end();
}

bool ProfessionnalContact::addAdditionnalEmail(const std::string& additionnalEmail) {
	std::lock_guard<std::mutex> lock(additionnalEmailsMutex);
	auto it = std::find(additionnalEmails.begin(), additionnalEmails.end(), additionnalEmail);
	if(it == additionnalEmails.end()) {
		additionnalEmails.push_back(additionnalEmail);
		return true;
	}
	return false;
}

bool ProfessionnalContact::removeAdditionnalEmail(const std::string& additionnalEmail) {
	std::lock_guard<std::mutex> lock(additionnalEmailsMutex);
	auto it = std::find(additionnalEmails.begin(), additionnalEmails.end(), additionnalEmail);
	if(it != additionnalEmails.end()) {
		additionnalEmails.erase(it);
		return true;
	}
	return false;
}

void ProfessionnalContact::clearAdditionnalEmails() {
	std::lock_guard<std::mutex> lock(additionnalEmailsMutex);
	additionnalEmails.clear();
}

const std::vector<std::weak_ptr<ProfessionnalContact>> ProfessionnalContact::getAssociates() {
	std::lock_guard<std::mutex> lock(associatesMutex);
	return associates;
}

bool ProfessionnalContact::hasAssociate(const std::weak_ptr<ProfessionnalContact> associate) {
	std::lock_guard<std::mutex> lock(associatesMutex);
	return std::find(associates.begin(), associates.end(), associate) != associates.end();
}

bool ProfessionnalContact::addAssociate(const std::weak_ptr<ProfessionnalContact> associate) {
	std::lock_guard<std::mutex> lock(associatesMutex);
	auto it = std::find(associates.begin(), associates.end(), associate);
	if(it == associates.end()) {
		associates.push_back(associate);
		return true;
	}
	return false;
}

bool ProfessionnalContact::removeAssociate(const std::weak_ptr<ProfessionnalContact> associate) {
	std::lock_guard<std::mutex> lock(associatesMutex);
	auto it = std::find(associates.begin(), associates.end(), associate);
	if(it != associates.end()) {
		associates.erase(it);
		return true;
	}
	return false;
}

void ProfessionnalContact::clearAssociates() {
	std::lock_guard<std::mutex> lock(associatesMutex);
	associates.clear();
}


ProfessionnalContact::ProfessionnalContact(std::unique_ptr<Person>person_, const std::string& phoneNumber_, const std::string& email_, const std::vector<std::string>& additionnalEmails_)
:Contact(std::move(person_), phoneNumber_, email_)
,additionnalEmails(additionnalEmails_)
{}

void ProfessionnalContact::showInfo()
{

}

