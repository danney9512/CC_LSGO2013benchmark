#include "alg_crossover.h"
#include "alg_population.h"
#include "alg_math.h"

//-------------------------
//  GDR
//-------------------------

bool GlobalDiscreteRecombination::operator() (Individual *c, const Population &pop, const size_t Pop_Size, double pc) const
{
	if (pop.size() == 0 || alg_math::randDouble(0.0, 1.0) > pc) return false;

	*c = pop[0];
	Individual::GeneVec &x = c->gene();
	size_t gene_len = x.size();

	for (size_t i = 0; i < gene_len; ++i)
	{
		int idx = alg_math::randDouble(0, (int) Pop_Size - 1);
		x[i] = pop[idx].gene()[i];
	}
	return true;
}

//-------------------------
//  LIR
//-------------------------

bool LocalIntermediateRecombination::operator() (Individual *c, const Individual &p1, const Individual &p2, double pc) const
{
	if (p1.gene().size() != p2.gene().size() || alg_math::randDouble(0.0, 1.0) > pc) return false;

	*c = p1;
	Individual::GeneVec &x = c->gene();
	size_t gene_len = x.size();

	for (size_t i = 0; i < gene_len; ++i)
	{
		x[i] = (p1.gene()[i] + p2.gene()[i]) / 2;
	}
	return true;
}