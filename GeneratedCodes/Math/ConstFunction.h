#ifndef __CONST_FUNCTION_H__
#define __CONST_FUNCTION_H__
#include <string>
#include <map>
#include <memory>

#include "Function.h"

class ConstFunction: virtual public Function
{
	double constant;

public: 
	const double& getConstant() const;


private:
	ConstFunction(const double& constant_);
public:
	virtual ~ConstFunction() {}

	virtual double solve(std::map<std::string,double>& variables);
	virtual std::unique_ptr<Function> derivative(std::string variable);
};

#endif

