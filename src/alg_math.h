#ifndef ALGMATH__
#define ALGMATH__

#include <cstdlib>
#include <vector>
#include "alg_individual.h"
#include "problem.h"
//#include "problem_test_functions.h"

namespace alg_math 
{
	inline double square(double n) { return n*n; }
	inline double randDouble(double lb, double ub) { return lb + (static_cast<double>(std::rand()) / RAND_MAX)*(ub - lb); }
	inline int randInt(int lb, int ub) { return lb + std::rand() % (ub - lb + 1); }
}


#endif // !ALGMATH__

