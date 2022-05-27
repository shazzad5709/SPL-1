#ifndef __MATRIX_FUNCTION_H__
#define __MATRIX_FUNCTION_H__
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Function.h"

class MatrixFunction: virtual public Function
{

	MatrixFunction(std::vector<std::vector<std::shared_ptr<Function>>> functions);
public:
	virtual ~MatrixFunction() {}

	virtual std::shared_ptr<Function> solve(std::map<std::string,std::shared_ptr<Function>>& variables);
	virtual std::unique_ptr<Function> derivative(std::string variable);
};

#endif

