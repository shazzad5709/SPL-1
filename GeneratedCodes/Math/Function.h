#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <string>
#include <map>
#include <memory>

class Function
{
protected:
	std::size_t type;

public: 
	const std::size_t& getType() const;


	virtual ~Function() {}

	virtual double solve(std::map<std::string,double>& variables) = 0;
	virtual std::unique_ptr<Function> derivative(std::string variable) = 0;
};

#endif

