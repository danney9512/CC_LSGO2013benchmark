#include "alg_initialization.h"
#include "problem.h"
#include "alg_individual.h"
#include "alg_population.h"
#include "alg_math.h"

#include <cstddef>
using std::size_t;

#include <iostream>
using namespace std;

TRandomInitialization RandomInitialization;

void TRandomInitialization::operator()(Individual *indv, const CProblem &prob) const
{
	Individual::GeneVec &x = indv->gene();
	x.resize(prob.dim());

	for (size_t i = 0; i<x.size(); i += 1)
	{
		x[i] = alg_math::randDouble(prob.lower_bound(), prob.upper_bound());
	}
}
// ----------------------------------------------------------------------
void TRandomInitialization::operator()(Population *pop, const CProblem &prob) const
{
	for (size_t i = 0; i<pop->size(); i += 1)
	{
		(*this)(&(*pop)[i], prob);
	}
}
