#include "CC_decomposer.h"
#include "problem.h"
#include "alg_math.h"
#include "alg_L-SHADE.h"
#include "alg_mL-SHADE.h"

Decomposer::Decomposer(std::string& optimizer_name, std::ifstream& optimizer_ifile, Group& g, const int pop_size)
{
	group_ = g;
	pop_size_ = pop_size;

	//population_ 初始化為 0
	population_ = std::vector<CC_SubComponent>(pop_size_, CC_SubComponent(g.size(), 0.0));

	if (optimizer_name == "mL-SHADE")
	{
		optimizer_ = new mL_SHADE();

		if (!optimizer_ifile)
			return;
		else
			optimizer_->Setup(optimizer_ifile);
	}

	mutation_opt_ = "";
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

Individual& Decomposer::Optimize(const int iteration,const CProblem& prob, Individual& context_vec, unsigned long long int& nfe, const unsigned long long int& MAX_nFE)
{
	// TODO: 真正 mL-SHADE 在執行的地方
	optimizer_->Solve(context_vec, group_, population_, prob, iteration, pop_size_, nfe, MAX_nFE);
	// TODO: 預留 多個 不同的 Mutation operator


	return context_vec;
}

