#include "alg_individual.h"
#include "problem.h"

const CProblem * Individual::target_problem_ = 0;

Individual::Individual(std::size_t gene_len, double fit_val):
	gene_(gene_len), fitness_(fit_val)
{
	if (target_problem_ != 0)
	{
		gene_.resize(target_problem_->dim());
	}
}


std::ostream & operator << (std::ostream &os, const Individual &indv)
{
	os << "gene value : ";
	for (int i = 0; i<indv.gene().size(); ++i)
	{
		os << indv.gene()[i] << ' ';
	}
	os << " # fitness : " << indv.fitness();
	return os;
}