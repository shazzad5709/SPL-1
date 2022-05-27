#ifndef __VAR_FUNCTION_H__
#define __VAR_FUNCTION_H__
#include <string>
#include <map>
#include <memory>

#include "Function.h"

class VarFunction: virtual public Function
{
	std::string variable;

public: 
	const std::string& getVariable() const;


private:
	VarFunction(const std::string& variable_);
public:
	virtual ~VarFunction() {}

	virtual double solve(std::map<std::string,double>& variables);
	virtual std::unique_ptr<Function> derivative(std::string variable);
};

#endif

