#include "CC_decomposer.h"
#include "problem.h"
#include "alg_math.h"

void SubComponentInitialization(std::vector<double>& sub_component, const CProblem& prob)
{
	for (int i = 0; i < sub_component.size(); i += 1)
	{
		sub_component[i] = alg_math::randDouble(prob.lower_bound(), prob.upper_bound());
	}
}

void Decomposer::PopulationInitialization(const CProblem& prob)
{
	for (int i = 0; i < pop_size_; i += 1)
	{
		SubComponentInitialization(population_[i], prob);
	}
}

