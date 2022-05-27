#include "Contact.h"

const std::string& Contact::getPhoneNumber() {
	std::lock_guard<std::mutex> lock(phoneNumberMutex);
	return phoneNumber;
}

void Contact::setPhoneNumber(const std::string& phoneNumber_) {
	std::lock_guard<std::mutex> lock(phoneNumberMutex);
	phoneNumber = phoneNumber_;
}

const std::string& Contact::getEmail() {
	std::lock_guard<std::mutex> lock(emailMutex);
	return email;
}

void Contact::setEmail(const std::string& email_) {
	std::lock_guard<std::mutex> lock(emailMutex);
	email = email_;
}

const Date& Contact::getCreationDate() const  {
	return *creationDate;
}

const Person& Contact::getPerson()  {
	std::lock_guard<std::mutex> lock(personMutex);
	return *person;
}

void Contact::setPerson(std::unique_ptr<Person> person_) {
	std::lock_guard<std::mutex> lock(personMutex);
	person = std::move(person_);
}


Contact::Contact(std::unique_ptr<Person>person_, const std::string& phoneNumber_, const std::string& email_)
:person(std::move(person_))
,phoneNumber(phoneNumber_)
,email(email_)
{}

void Contact::showInfo()
{

}

