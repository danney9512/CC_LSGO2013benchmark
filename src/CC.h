#ifndef CC_FRAMEWORK__
#define CC_FRAMEWORK__

#include "alg_individual.h"
#include "CC_decomposer.h"

class CC_alg
{
public:
	explicit CC_alg();

	void Setup(GroupsResult& all_groups, std::ifstream& optimizer_ifile);
	Individual& Solve(const CProblem& prob);

private:
	Individual context_vector_;

	std::vector<Decomposer> decomposers_;

	unsigned long long int nFE_;
	unsigned long long int max_nFE_;

	int optimizer_LearningPeriod;
};


#endif // !CC__

