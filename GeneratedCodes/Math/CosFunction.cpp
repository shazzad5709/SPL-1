#include "CosFunction.h"

const std::shared_ptr<Function> CosFunction::getFunction() const {
	return function;
}


CosFunction::CosFunction(const std::shared_ptr<Function> function_)
:function(function_)
{}

std::shared_ptr<Function> CosFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
	std::shared_ptr<Function> autoGen;

	return autoGen;
}

std::unique_ptr<Function> CosFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

