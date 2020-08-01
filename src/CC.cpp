#include "CC.h"
#include "2013LSGOBenchmarks/2013Benchmarks.h"
#include "alg_math.h"

CC_alg::CC_alg(const unsigned long long int used_nfe, unsigned long long int max_nFE)
{
	nFE_ = used_nfe;
	max_nFE_ = max_nFE;

	optimizer_LearningPeriod_ = 0;
	pop_size_ = 0;
}

void CC_alg::Setup(std::ifstream& CC_ifile, GroupsResult& all_groups, std::string& optimizer_name, const std::string& optimizer_path)
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
		Decomposer dp(all_groups.sep_groups()[i], pop_size_);
		decomposers_.push_back(dp);
	}
	for (int i = 0; i < nonsep_num; ++i)
	{
		Decomposer dp(all_groups.nonsep_groups()[i], pop_size_);
		decomposers_.push_back(dp);
	}
	
	for (size_t i = 0; i < decomposers_.size(); ++i)
	{
		decomposers_[i].SetupOptimizer(optimizer_name, optimizer_path, max_nFE_);
	}
	
}


Individual CC_alg::Solve(const CProblem& prob)
{
	// Set log system and target problem

	// TODO: log setting


	// Set parameter and random generator
	unsigned long long int nFE_now = nFE_;
	const unsigned long long int MAX_nFE = max_nFE_;

	std::string MutationOperator = "Cur-to-pbest";

	// Create Benchmark function
	Benchmarks* fp = generateFuncObj(prob.id(), prob.lower_bound(), prob.upper_bound(), prob.dim());
	fp->nextRun();


	// Random Initialize populations in decomposer  and  Initialize memory system
	for (int i = 0; i < decomposers_.size(); ++i)
	{
		decomposers_[i].PopulationInitialization(prob);
	}

	// Initialize Context vector
	context_vector_ = Individual(prob.dim(), 0.0);

	// Randomly choose subcomponent gene from decomposers into context vector
	for (int i = 0; i < decomposers_.size(); i += 1)
	{
		int rand_pop_num = alg_math::randInt(0, pop_size_ - 1);
		for (int j = 0; j < decomposers_[i].group().size(); j += 1)
		{
			context_vector_.gene()[decomposers_[i].group()[j]] = decomposers_[i].population()[rand_pop_num][j];
		}
	}
	// Evaluate context vector and update its fitness value
	context_vector_.fitness() = fp->compute(context_vector_.gene());
	++nFE_now;


	
	// CC evaluation
	size_t cycle_cnt = 0;
	while (nFE_now < MAX_nFE)
	{
		cycle_cnt += 1;

		for (int i = 0; i < decomposers_.size(); ++i)
		{
			decomposers_[i].setMutationOperator(MutationOperator);

			cout << "Decomposer[ " << i << "]   ";
			// Optimize each SubComponents
			context_vector_ = decomposers_[i].Optimize(optimizer_LearningPeriod_, prob, fp, context_vector_, nFE_now, decomposers_.size());
			
			//cout << "Group " << i << "  ,nFE = " << nFE_now << endl;			
		}
	}
	
	return context_vector_;
}