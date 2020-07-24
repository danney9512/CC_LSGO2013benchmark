#ifndef CC_DECOMPOSER__
#define CC_DECOMPOSER__

#include <vector>
#include <fstream>

#include "CC_group.h"
#include "alg_individual.h"
#include "alg_base.h"


class Decomposer
{
public:
	typedef std::vector<double> CC_SubComponent;

	explicit Decomposer(std::string& optimizer_name, std::ifstream& optimizer_ifile, Group& g, const int pop_size);


	Individual& Optimize(const CProblem& prob, Individual& context_vec, unsigned long long int& nfe, const unsigned long long int& MAX_nFE, const std::string& mutation_opt);
	void PopulationInitialization(const CProblem& prob);

	void setMutationOperator(std::string opt) { mutation_opt_ = opt; }

	~Decomposer() { delete optimizer_; }
private:
	Group group_;

	int pop_size_;
	std::vector<CC_SubComponent> population_;
	
	BaseEA *optimizer_ = nullptr;

	std::string mutation_opt_;
};

#endif // ! CC_DECOMPOSER__

