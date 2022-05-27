#ifndef __POW_FUNCTION_H__
#define __POW_FUNCTION_H__
#include <string>
#include <map>
#include <memory>

#include "Function.h"

class PowFunction: virtual public Function
{
	std::shared_ptr<Function> theBase;
	std::shared_ptr<Function> exponent;

	PowFunction(const std::shared_ptr<Function> exponent_);
public:
	virtual ~PowFunction() {}

	virtual double solve(std::map<std::string,double>& variables);
	virtual std::unique_ptr<Function> derivative(std::string variable);
};

#endif

