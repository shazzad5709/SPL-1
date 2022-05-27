#include "MulFunction.h"


MulFunction::MulFunction(const std::vector<std::shared_ptr<Function>>& functions_)
:functions(functions_)
{}

double MulFunction::solve(std::map<std::string,double>& variables)
{
	double autoGen;

	return autoGen;
}

std::unique_ptr<Function> MulFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

