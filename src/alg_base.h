#ifndef ALGORITHM_BASE__
#define ALGORITHM_BASE__

#include <string>
#include <iosfwd>

class Population;
class CProblem;
 
class BaseEA
{
public:
	BaseEA(const std::string &name) :name_(name) {}

	virtual void Setup(std::ifstream &ifile) = 0;
	virtual void Solve(Population *solutions, const CProblem &prob) = 0;

	const std::string & name() const
	{
		return name_;
	}

protected:
	std::string name_{};

};

#endif // !ALGORITHM_BASE__
