
#include "experiment.h"
#include <string>

Experiment::Experiment(std::string algo_str = "", std::string grouping = "", std::string optimizer="", std::string CB = "", size_t pid = 0, unsigned long long int nFE = 0)
{
	algo_name_ = algo_str;
	grouping_ = grouping;
	optimizer_ = optimizer;
	contributationBase = CB;
	problem_id_ = pid;
	max_nFE_ = nFE;
}

ExperimentSet::ExperimentSet(std::ifstream &ifile)
{
	std::string algo_name, grouping , optimizer, CB;
	size_t pid = 0;
	unsigned long long int max_nfe = 0;

	std::string dummy;
	while (ifile >> dummy)
	{
		ifile >> dummy >> dummy >> algo_name;
		ifile >> dummy >> dummy >> grouping;
		ifile >> dummy >> dummy >> optimizer;
		ifile >> dummy >> dummy >> CB;
		ifile >> dummy >> dummy >> pid;
		ifile >> dummy >> dummy >> max_nfe;
		exps.push_back(Experiment(algo_name, grouping, optimizer, CB, pid, max_nfe));
	}
}

std::ostream & operator << (std::ostream &os, const Experiment &exp)
{
	os << "Algorithm name: " << exp.algo_name() << " " << exp.optimizer() << "-" << exp.grouping() << " " << exp.CB() << std::endl;
	os << "Test function: " << exp.problem_id() << ", Max nFE = " << exp.max_nFE() << std::endl;

	return os;
}