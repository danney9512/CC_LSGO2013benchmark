#ifndef CC_FRAMEWORK__
#define CC_FRAMEWORK__

#include <fstream>

#include "experiment.h"
#include "problem.h"
#include "alg_individual.h"
#include "CC_decomposer.h"

class CC_alg
{
public:
	explicit CC_alg(const unsigned long long int used_nfe, Experiment& exp);

	void Setup(std::ifstream& CC_ifile, GroupsResult& all_groups, const std::string& optimizer_name, const std::string& optimizer_path);
	Individual Solve(const CProblem& prob);

private:
	Individual context_vector_;

	std::vector<Decomposer> decomposers_;

	unsigned long long int nFE_;
	unsigned long long int max_nFE_;

	int pop_size_;
	int optimizer_LearningPeriod_;

	string alg_name_,
		grouping_name_,
		optimizer_name_,
		CB_name_;
};


#endif // !CC__

