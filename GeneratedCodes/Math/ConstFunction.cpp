#include "ConstFunction.h"

const double& ConstFunction::getConstant() const {
	return constant;
}


ConstFunction::ConstFunction(const double& constant_)
:constant(constant_)
{}

double ConstFunction::solve(std::map<std::string,double>& variables)
{
	double autoGen;

	return autoGen;
}

std::unique_ptr<Function> ConstFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

