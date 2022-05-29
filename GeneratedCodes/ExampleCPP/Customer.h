#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__
#include <string>

#include "SimpleClass.h"
#include "Account.h"

class Customer: public SimpleClass
{
	std::string address;
	std::string phoneNum;
	int accountNum;

public:
	virtual ~Customer() {}

	virtual bool generalInquiry();
	virtual bool depositMoney();
	virtual bool withdrawMoney();
	virtual Account openAccount();
	virtual Account closeAccount();
	virtual bool applyForLoan();
	virtual bool requestCard();
};

#endif

