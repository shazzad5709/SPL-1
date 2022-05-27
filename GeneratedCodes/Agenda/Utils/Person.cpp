#include "Person.h"

const std::string& Person::getFirstName() const {
	return firstName;
}

const std::string& Person::getLastName() const {
	return lastName;
}

const Date& Person::getBirthday() const  {
	return *birthday;
}


Person::Person(const std::string& firstName_, const std::string& lastName_, std::unique_ptr<Date>birthday_)
:firstName(firstName_)
,lastName(lastName_)
,birthday(std::move(birthday_))
{}

void Person::showInfo()
{

}

