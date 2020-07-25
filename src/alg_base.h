#ifndef ALGORITHM_BASE__
#define ALGORITHM_BASE__

#include <string>
#include <iosfwd>
#include <vector>

class Population;
class CProblem;
class Group;
class Individual;
 
class BaseEA
{
public:
	BaseEA(const std::string &name) :name_(name) {}

	virtual void Setup(std::ifstream &ifile) = 0;
	virtual void Solve(Population *solutions, const CProblem &prob) = 0;
	virtual void Solve(Individual& context_vec, const Group& group, const std::vector<std::vector<double>>& population, const CProblem& prob, const int iteration, const int pop_size, unsigned long long int& nfe, const unsigned long long int max_nFE) = 0;
	
	const std::string & name() const
	{
		return name_;
	}

protected:
	std::string name_{};

};

#endif // !ALGORITHM_BASE__
