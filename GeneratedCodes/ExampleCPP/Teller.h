#ifndef __TELLER_H__
#define __TELLER_H__
#include "SimpleClass.h"
#include "Bank.h"

class Teller: public SimpleClass
{

public:
	virtual ~Teller() {}

	virtual bool collectMoney();
	virtual bool openAccount();
	virtual bool closeAccount();
	virtual bool loanRequest();
	virtual void provideInfo();
	virtual bool issueCard();
};

#endif

