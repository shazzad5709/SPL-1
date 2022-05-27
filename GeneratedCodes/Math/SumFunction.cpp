#include "SumFunction.h"


SumFunction::SumFunction(const std::vector<std::shared_ptr<Function>>& functions_)
:functions(functions_)
{}

double SumFunction::solve(std::map<std::string,double>& variables)
{
	double autoGen;

	return autoGen;
}

std::unique_ptr<Function> SumFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

