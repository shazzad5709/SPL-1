#include "SinFunction.h"

const std::shared_ptr<Function> SinFunction::getFunction() const {
	return function;
}


SinFunction::SinFunction(const std::shared_ptr<Function> function_)
:function(function_)
{}

std::shared_ptr<Function> SinFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
	std::shared_ptr<Function> autoGen;

	return autoGen;
}

std::unique_ptr<Function> SinFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

