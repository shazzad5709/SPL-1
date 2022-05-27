#ifndef __CONTACT_H__
#define __CONTACT_H__
#include <string>
#include <mutex>
#include <memory>

#include "../Properties/Showable.h"
#include "../Utils/Date.h"
#include "../Utils/Person.h"

class Contact: virtual public Showable
{
	std::string phoneNumber;
	std::mutex phoneNumberMutex;
	std::string email;
	std::mutex emailMutex;
	std::unique_ptr<Date> creationDate;
	std::unique_ptr<Person> person;
	std::mutex personMutex;

public: 
	const std::string& getPhoneNumber();
	void setPhoneNumber(const std::string& phoneNumber_);

	const std::string& getEmail();
	void setEmail(const std::string& email_);

	const Date& getCreationDate() const;

	const Person& getPerson();
	void setPerson(std::unique_ptr<Person> person_);


	Contact(std::unique_ptr<Person>person_, const std::string& phoneNumber_, const std::string& email_);
	virtual ~Contact() {}

	virtual void showInfo();
};

#endif

