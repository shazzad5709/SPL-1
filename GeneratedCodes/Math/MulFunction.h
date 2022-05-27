#ifndef __MUL_FUNCTION_H__
#define __MUL_FUNCTION_H__
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Function.h"

class MulFunction: virtual public Function
{
	std::vector<std::shared_ptr<Function>> functions;

	MulFunction(const std::vector<std::shared_ptr<Function>>& functions_);
public:
	virtual ~MulFunction() {}

	virtual double solve(std::map<std::string,double>& variables);
	virtual std::unique_ptr<Function> derivative(std::string variable);
};

#endif

