#ifndef __PERSON_H__
#define __PERSON_H__
#include <string>
#include <memory>

#include "Date.h"
#include "../Properties/Showable.h"

class Person: virtual public Showable
{
	std::string firstName;
	std::string lastName;
	std::unique_ptr<Date> birthday;

public: 
	const std::string& getFirstName() const;

	const std::string& getLastName() const;

	const Date& getBirthday() const;


	Person(const std::string& firstName_, const std::string& lastName_, std::unique_ptr<Date>birthday_);
	virtual ~Person() {}

	virtual void showInfo();
};

#endif

