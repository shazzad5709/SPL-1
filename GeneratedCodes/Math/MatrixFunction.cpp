#include "MatrixFunction.h"


MatrixFunction::MatrixFunction(std::vector<std::vector<std::shared_ptr<Function>>> functions)
{}

std::shared_ptr<Function> MatrixFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
	std::shared_ptr<Function> autoGen;

	return autoGen;
}

std::unique_ptr<Function> MatrixFunction::derivative(std::string variable)
{
	std::unique_ptr<Function> autoGen;

	return autoGen;
}

