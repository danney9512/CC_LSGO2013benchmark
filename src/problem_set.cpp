#include "problem_set.h"

#include <string>

CPromblemSet::CPromblemSet(std::ifstream &ifile)
{
	int ID;
	std::string name;
	int dim;
	double lower_bound, upper_bound, global_optimum;

	std::string dummy; 
	while (ifile >> dummy)
	{
		ifile >> dummy >> dummy >> ID;
		ifile >> dummy >> dummy >> name;
		ifile >> dummy >> dummy >> dim;
		ifile >> dummy >> dummy >> lower_bound;
		ifile >> dummy >> dummy >> upper_bound;
		ifile >> dummy >> dummy >> global_optimum;
		problems_.push_back( CProblem(ID, name, dim, lower_bound, upper_bound, global_optimum) );
	}
}