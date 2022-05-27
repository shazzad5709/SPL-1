#include "CEO.h"

const std::string& CEO::getCompanyName() const {
	return companyName;
}


CEO::CEO(std::unique_ptr<Person>person_, const std::string& phoneNumber_, const std::string& email_, const std::vector<std::string>& additionnalEmails_, const std::string& companyName_)
:ProfessionnalContact(std::move(person_), phoneNumber_, email_, additionnalEmails_)
,companyName(companyName_)
{}

