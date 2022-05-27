#ifndef __LOG_FUNCTION_H__
#define __LOG_FUNCTION_H__
#include <string>
#include <map>
#include <memory>

#include "Function.h"

class LogFunction: virtual public Function
{
	std::shared_ptr<Function> function;

	LogFunction(const std::shared_ptr<Function> function_);
public:
	virtual ~LogFunction() {}

	virtual double solve(std::map<std::string,double>& variables);
	virtual std::unique_ptr<Function> derivative(std::string variable);
};

#endif

