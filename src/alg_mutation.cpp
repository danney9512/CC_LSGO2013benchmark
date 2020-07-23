#include "alg_mutation.h"
#include "alg_math.h"
#include "alg_individual.h"
#include "problem.h"

#include <cmath>
#include <random>

bool GaussianMutation::operator()(Individual *c, double pm, double mu, double sigma) const
{
	bool isMutated = false;
	Individual::GeneVec &x = c->gene();
	double ub = Individual::TargetProblem().upper_bound(),
		   lb = Individual::TargetProblem().lower_bound();

	std::default_random_engine generator;

	for (size_t i = 0; i < x.size(); ++i)
	{
		if (alg_math::randDouble(0.0, 1.0) <= pm)
		{
			isMutated = true;
			std::normal_distribution<double> distribution(mu, sigma);
			x[i] += distribution(generator);
			x[i] = std::min(ub, std::max(lb, x[i]));
		}
	}
	return isMutated;
}


bool PolynomialMutation::operator()(Individual *c, double pm, double eta) const
{
	bool mutated = false;

	Individual::GeneVec &x = c->gene();

	for (size_t i = 0; i < x.size(); i += 1)
	{
		if (alg_math::randDouble(0.0, 1.0) <= pm)
		{
			mutated = true;

			double y = x[i],
				   lb = Individual::TargetProblem().lower_bound(),
				   ub = Individual::TargetProblem().upper_bound();

			double delta1 = (y - lb) / (ub - lb),
				delta2 = (ub - y) / (ub - lb);

			double mut_pow = 1.0 / (eta + 1.0);

			double rnd = alg_math::randDouble(0.0, 1.0), deltaq = 0.0;
			if (rnd <= 0.5)
			{
				double xy = 1.0 - delta1;
				double val = 2.0*rnd + (1.0 - 2.0*rnd)*(pow(xy, (eta + 1.0)));
				deltaq = pow(val, mut_pow) - 1.0;
			}
			else
			{
				double xy = 1.0 - delta2;
				double val = 2.0*(1.0 - rnd) + 2.0*(rnd - 0.5)*(pow(xy, (eta + 1.0)));
				deltaq = 1.0 - (pow(val, mut_pow));
			}

			y = y + deltaq * (ub - lb);
			y = std::min(ub, std::max(lb, y));

			x[i] = y;
		}
	}
	return mutated;
}