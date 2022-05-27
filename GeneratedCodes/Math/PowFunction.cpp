#include "PowFunction.h"


PowFunction::PowFunction(const std::shared_ptr<Function> exponent_)
:exponent(exponent_)
{}

double PowFunction::solve(std::map<std::string,double>& variables)
{
	double autoGen;

	return autoGen;
}

std::unique_ptr<Function> PowFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

