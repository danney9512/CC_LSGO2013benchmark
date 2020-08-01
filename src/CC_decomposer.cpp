#include "CC_decomposer.h"
#include "problem.h"
#include "alg_math.h"


Decomposer::Decomposer(Group& g, const int pop_size)
{
	group_ = g;
	pop_size_ = pop_size;

	// Initialization the population_ to zero
	population_ = std::vector<CC_SubComponent>(pop_size_, CC_SubComponent(g.size(), 0.0));

	optimizer_ = nullptr;
	mutation_opt_ = "";
}

Decomposer::Decomposer(const std::string& optimizer_name, const std::string& optimizer_path, Group& g, const int pop_size)
{
	group_ = g;
	pop_size_ = pop_size;

	// Initialization the population_ to zero
	population_ = std::vector<CC_SubComponent>(pop_size_, CC_SubComponent(g.size(), 0.0));

	optimizer_ = nullptr;
	mutation_opt_ = "";


	/////  XMEMORY ERROR  /////
	/*
	std::ifstream optimizer_ifile(optimizer_path);
	if (optimizer_name == "mL-SHADE")
	{
		optimizer_ = new mL_SHADE();

		if (!optimizer_ifile)
			return;
		else
			optimizer_->Setup(optimizer_ifile);
	}
	optimizer_ifile.close();
	*/
	/////  XMEMORY ERROR /////
}

void Decomposer::SetupOptimizer(const std::string& optimizer_name, const std::string& optimizer_path, const unsigned long long int maxFE)
{
	std::ifstream optimizer_ifile(optimizer_path);
	if (optimizer_name == "mL-SHADE")
	{
		optimizer_ = new CC_mL_SHADE(pop_size_, maxFE);

		if (!optimizer_ifile)
		{
			return;
		}
		else
		{
			optimizer_->Setup(optimizer_ifile);
		}
			
	}
	optimizer_ifile.close();
}

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

Individual Decomposer::Optimize(const int iteration, const CProblem& prob, Benchmarks* objfunction,  Individual& context_vec, unsigned long long int& nfe, const size_t num_deps)
{
	// record the nfe value of each decomposer
	unsigned long long int nfe_before_optimize = nfe;
	
	// Running optimizer mL-SHADE
	Individual new_context_vector;
	new_context_vector = optimizer_->Solve(context_vec, group_, population_, prob, objfunction, iteration, nfe, used_nfe_, num_deps);

	used_nfe_ = nfe - nfe_before_optimize;
	
	return new_context_vector;
}

