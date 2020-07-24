#ifndef CC_FRAMEWORK__
#define CC_FRAMEWORK__

#include <fstream>

#include "problem.h"
#include "alg_individual.h"
#include "CC_decomposer.h"

class CC_alg
{
public:
	explicit CC_alg(const unsigned long long int used_nfe, unsigned long long int max_nFE);

	void Setup(std::ifstream& CC_ifile, GroupsResult& all_groups, std::string& optimizer_name, std::ifstream& optimizer_ifile);
	Individual& Solve(const CProblem& prob);

private:
	Individual context_vector_;

	std::vector<Decomposer> decomposers_;

	unsigned long long int nFE_;
	unsigned long long int max_nFE_;

	int pop_size_;
	int optimizer_LearningPeriod_;
};


#endif // !CC__

