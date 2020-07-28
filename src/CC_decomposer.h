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

	explicit Decomposer(Group& g, const int pop_size);
	explicit Decomposer(const std::string& optimizer_name, const std::string& optimizer_path, Group& g, const int pop_size);

	void SetupOptimizer(const std::string& optimizer_name, const std::string& optimizer_path, const unsigned long long int maxFE);
	void PopulationInitialization(const CProblem& prob);

	Individual& Optimize(const int iteration, const CProblem& prob, Individual& context_vec, unsigned long long int& nfe, const unsigned long long int& MAX_nFE);
	
	void setMutationOperator(std::string opt) { mutation_opt_ = opt; }

	const Group& group() const { return group_; }
	const std::vector<CC_SubComponent>& population() const { return population_; }

	~Decomposer() { if (optimizer_ != nullptr)	delete optimizer_; }
private:
	Group group_;

	int pop_size_;
	std::vector<CC_SubComponent> population_;
	
	BaseEA *optimizer_ = nullptr;
	std::string mutation_opt_;
};

#endif // ! CC_DECOMPOSER__

