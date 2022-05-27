#ifndef __SUM_FUNCTION_H__
#define __SUM_FUNCTION_H__
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Function.h"

class SumFunction: virtual public Function
{
	std::vector<std::shared_ptr<Function>> functions;

	SumFunction(const std::vector<std::shared_ptr<Function>>& functions_);
public:
	virtual ~SumFunction() {}

	virtual double solve(std::map<std::string,double>& variables);
	virtual std::unique_ptr<Function> derivative(std::string variable);
};

#endif

