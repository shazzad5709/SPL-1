#ifndef __BANK_H__
#define __BANK_H__
#include <string>

#include "Teller.h"
#include "Customer.h"

class Bank
{
	int bankID;
	std::string name;
	std::string address;
	Teller tellers;
	Customer customers;

public: 



};

#endif

