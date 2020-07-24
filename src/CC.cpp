#include "CC.h"
#include "2013LSGOBenchmarks/2013Benchmarks.h"

CC_alg::CC_alg(const unsigned long long int used_nfe, unsigned long long int max_nFE)
{
	nFE_ = used_nfe;
	max_nFE_ = max_nFE_;

	optimizer_LearningPeriod_ = 0;
	pop_size_ = 0;
}

void CC_alg::Setup(std::ifstream& CC_ifile, GroupsResult& all_groups, std::string& optimizer_name, std::ifstream& optimizer_ifile)
{
	if (!CC_ifile) return;

	// Loading and Setting CC
	std::string dummy;
	CC_ifile >> dummy >> dummy >> pop_size_;
	CC_ifile >> dummy >> dummy >> optimizer_LearningPeriod_;

	// Initialize vector<decomposers>
	int sep_num = all_groups.sep_groups().size();
	int nonsep_num = all_groups.nonsep_groups().size();

	for (int i = 0; i < sep_num; ++i)
	{
		Decomposer dp(optimizer_name, optimizer_ifile, all_groups.sep_groups()[i], pop_size_);
		decomposers_.push_back(dp);
	}
	for (int i = 0; i < nonsep_num; ++i)
	{
		Decomposer dp(optimizer_name, optimizer_ifile, all_groups.nonsep_groups()[i], pop_size_);
		decomposers_.push_back(dp);
	}

}


Individual& CC_alg::Solve(const CProblem& prob)
{
	// Set log system and target problem

	// TODO: log setting


	// Set parameter and random generator
	unsigned long long int nFE_now = nFE_;
	const unsigned long long int MAX_nFE = max_nFE_;

	std::string MutationOperator = "Cur-to-pbest";

	// Initialize Context vector
	context_vector_ = Individual(prob.dim(), 0.0);

	// TODO: randomly choose subcomponent gene from decomposers into context vector
	// TODO: update the fitness of context vector


	// Create Benchmark function
	Benchmarks* fp = generateFuncObj(prob.id(), prob.lower_bound(), prob.upper_bound(), prob.dim());
	fp->nextRun();


	// Random Initialize populations in decomposer  and  Initialize memory system
	for (int i = 0; i < decomposers_.size(); ++i)
	{
		decomposers_[i].PopulationInitialization(prob);

		// TODO: Initialize the mL-SHADE memory system
	}

	// CC evaluation
	size_t cycle_cnt = 0;
	while (nFE_now < MAX_nFE)
	{
		cycle_cnt += 1;

		for (int i = 0; i < decomposers_.size(); ++i)
		{
			
			for (int j = 0; j < optimizer_LearningPeriod_; ++j)
			{
				decomposers_[i].setMutationOperator(MutationOperator);

				// TODO: Optimize each SubComponents


			}
			// TODO: Update context vector gene and fitness
		}
	}

	return context_vector_;
}