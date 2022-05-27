#ifndef __SIN_FUNCTION_H__
#define __SIN_FUNCTION_H__
#include <string>
#include <map>
#include <memory>

#include "Function.h"

class SinFunction: virtual public Function
{
	std::shared_ptr<Function> function;

public: 
	const std::shared_ptr<Function> getFunction() const;


private:
	SinFunction(const std::shared_ptr<Function> function_);
public:
	virtual ~SinFunction() {}

	virtual std::shared_ptr<Function> solve(std::map<std::string,std::shared_ptr<Function>>& variables);
	virtual std::unique_ptr<Function> derivative(std::string variable);
};

#endif

