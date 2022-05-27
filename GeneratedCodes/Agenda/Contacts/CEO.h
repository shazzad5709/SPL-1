#ifndef __C_E_O_H__
#define __C_E_O_H__
#include <string>

#include "ProfessionnalContact.h"

class CEO: public ProfessionnalContact
{
	std::string companyName;

public: 
	const std::string& getCompanyName() const;


	CEO(std::unique_ptr<Person>person_, const std::string& phoneNumber_, const std::string& email_, const std::vector<std::string>& additionnalEmails_, const std::string& companyName_);
};

#endif

