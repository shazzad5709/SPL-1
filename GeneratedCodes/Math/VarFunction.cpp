#include "VarFunction.h"

const std::string& VarFunction::getVariable() const {
	return variable;
}


VarFunction::VarFunction(const std::string& variable_)
:variable(variable_)
{}

double VarFunction::solve(std::map<std::string,double>& variables)
{
	double autoGen;

	return autoGen;
}

std::unique_ptr<Function> VarFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

