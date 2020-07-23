#ifndef CC_DECOMPOSER__
#define CC_DECOMPOSER__

#include <vector>
#include "CC_group.h"

#include "alg_base.h"


class Decomposer
{
public:
	typedef std::vector<double> CC_SubComponent;

	explicit Decomposer(const std::string& optimizerNanme, Group& g);

	Individual& Optimize(const CProblem& prob, unsigned long long int& nfe, const std::string& mutation_opt);


	~Decomposer() {}
private:
	Group group_;

	int pop_size_;
	std::vector<CC_SubComponent> population_;
	
	BaseEA *optimizer_;

	std::string mutation_opt;
};

#endif // ! CC_DECOMPOSER__

