#include "LogFunction.h"


LogFunction::LogFunction(const std::shared_ptr<Function> function_)
:function(function_)
{}

double LogFunction::solve(std::map<std::string,double>& variables)
{
	double autoGen;

	return autoGen;
}

std::unique_ptr<Function> LogFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

