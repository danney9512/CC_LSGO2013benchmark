#include "CC_mL-SHADE.h"
#include "alg_population.h"
#include "alg_initialization.h"
#include "alg_mutation.h"
#include "alg_crossover.h"
#include "alg_math.h"
#include "problem.h"
#include "alg_log.h"
#include "CC_group.h"

#include <string>
#include <cstdlib>
#include <cmath>
#include <random>
#include <iostream>
#include <climits>
#include <algorithm>
using namespace std;

CC_mL_SHADE::CC_mL_SHADE(const size_t NP, const unsigned long long int maxFE)
{ 
	p_ = 0.11;
	rarc_ = 2.6;
	finit_ = 0.5;
	crinit_ = 0.5;
	scalemax_ = 0.2;
	h_ = 6;
	rNinit_ = 18;
	nmin_ = 4;
	
	name_ = "mL-SHADE";
	max_nfe_ = maxFE;

	NP_init_ = NP;
	NP_ = NP;
	A_ = (size_t)(NP_ * rarc_);

	memory_sys_ = MemorySystem(h_);

	multi_operator_num_ = 3;

	NP_op_ = std::vector<size_t>{ NP / multi_operator_num_,NP / multi_operator_num_,NP - (NP / multi_operator_num_) * (multi_operator_num_ - 1) };
}

void CC_mL_SHADE::Setup(std::ifstream& ifile)
{
	if (!ifile) return;

	// Loading and Setting
	string dummy;
	ifile >> dummy >> dummy >> name_;
	ifile >> dummy >> dummy >> rNinit_;
	ifile >> dummy >> dummy >> nmin_;
	ifile >> dummy >> dummy >> finit_;
	ifile >> dummy >> dummy >> crinit_;
	ifile >> dummy >> dummy >> rarc_;
	ifile >> dummy >> dummy >> p_;
	ifile >> dummy >> dummy >> h_;
	ifile >> dummy >> dummy >> scalemax_;

	// Initialize Memory Ststem
	memory_sys_ = MemorySystem(h_);
	for (size_t i = 0; i < h_; ++i)
	{
		memory_sys_[i] = Memory(finit_, crinit_);
	}

	// Initialize Population and Archive
	A_ = (size_t)(NP_ * rarc_);
}

void CC_mL_SHADE::MemorySystem::update_memory(const std::vector<Memory>& success_parameter, const std::vector<double>& success_fit_dif)
{
	if (success_parameter.size() != 0)
	{
		// Calculate weights for each success parameter
		double total_improve = 0.0;
		for (size_t i = 0; i < success_fit_dif.size(); ++i) total_improve += success_fit_dif[i];
		std::vector<double> weight(success_fit_dif.size());
		for (size_t i = 0; i < success_fit_dif.size(); ++i) weight[i] = success_fit_dif[i] / total_improve;

		// weighted Lehmer mean
		double pow_sum_f = 0.0, sum_f = 0.0, pow_sum_cr = 0.0, sum_cr = 0.0;
		for (size_t i = 0; i < success_parameter.size(); ++i)
		{
			pow_sum_f += weight[i] * success_parameter[i].F() * success_parameter[i].F();
			sum_f += weight[i] * success_parameter[i].F();

			pow_sum_cr += weight[i] * success_parameter[i].CR() * success_parameter[i].CR();
			sum_cr += weight[i] * success_parameter[i].CR();
		}

		// Update memory
		if (sum_f == 0) memories_[k_].F() = 0.0;
		else memories_[k_].F() = pow_sum_f / sum_f;
		if (sum_cr == 0 || memories_[k_].CR() == -1.0) memories_[k_].CR() = -1.0;
		else memories_[k_].CR() = pow_sum_cr / sum_cr;
		++k_;
		if (k_ >= h_) k_ = 0;
	}
}

void CC_mL_SHADE::MemorySystem::pertub_memory()
{
	memories_[k_].F() = alg_math::randDouble(0.0, 1.0);
	memories_[k_].CR() = alg_math::randDouble(0.0, 1.0);
	++k_;
	if (k_ >= h_) k_ = 0;
}

double EuclideanDistance(const Individual& ind1, const Individual& ind2)
{
	double ED = 0;
	for (int i = 0; i < ind1.gene().size(); i += 1)
	{
		ED += pow(ind1.gene()[i] - ind2.gene()[i], 2);
	}
	return sqrt(ED);
}

void CC_mL_SHADE::DEStrategy(const Population& bigpop, Population& pop, Population& archive_pop, std::vector<Memory>& sample_parameter, std::vector<Memory>& success_parameter, std::vector<double>& success_fit_dif, Benchmarks* fp, unsigned long long int& nfe, unsigned long long int& nfe_local, const string& mut_opt, const CProblem& prob)
{
	if (pop.size() == 0)
		return;

	const double pbest = p_, ub = prob.upper_bound(), lb = prob.lower_bound(), scalemax = scalemax_;
	const size_t Gene_Len = prob.dim(), H = h_;
	const unsigned long long int Max_NFE = max_nfe_;
	std::default_random_engine generator;

	// Set variables for terminal criteria
	bool stop_flag = false;
	size_t stop_idx = 0;

	if (nfe >= Max_NFE)
	{
		return;
	}

	Population children(pop.size());

	for (size_t i = 0; i < pop.size(); ++i)
	{
		// Generate CRi and Fi
		int r = alg_math::randInt(0, (int)(H - 1));

		double CRi;
		if (memory_sys_[r].CR() == -1.0)
			CRi = 0.0;
		else
		{
			std::normal_distribution<double> randn(memory_sys_[r].CR(), 0.1);
			CRi = std::min(1.0, std::max(0.0, randn(generator)));
		}

		std::cauchy_distribution<double> randc(memory_sys_[r].F(), 0.1);
		double Fi = randc(generator);
		while (Fi <= 0.0) Fi = randc(generator);
		if (Fi > 1.0) Fi = 1.0;


		sample_parameter.push_back(Memory(Fi, CRi));


		// DE Mutation
		Individual::GeneVec donor_vector;
		if (mut_opt == "Cur-to-pbest")
			donor_vector = CurtopBest_DonorVec((int)i, pbest, Fi, bigpop, archive_pop);
		else if (mut_opt == "Cur-to-best")
			donor_vector = CurtoBest_DonorVec((int)i, Fi, bigpop);
		else if (mut_opt == "Cur-to-grbest")
			donor_vector = CurtogrBest_DonorVec((int)i, pbest, Fi, bigpop);
		

		//cout << donor_vector.size() << endl;

		// DE crossover
		int R = alg_math::randInt(0, (int)(Gene_Len - 1));
		for (size_t k = 0; k < Gene_Len; ++k)
		{
			children[i].gene()[k] = (alg_math::randDouble(0.0, 1.0) <= CRi || k == R) ? donor_vector[k] : pop[i].gene()[k];

			// Repair scheme
			if (children[i].gene()[k] > ub) children[i].gene()[k] = (pop[i].gene()[k] + ub) / 2;
			else if (children[i].gene()[k] < lb) children[i].gene()[k] = (pop[i].gene()[k] + lb) / 2;
		}
		// Evaluate the individual's fitness
		children[i].fitness() = fp->compute(children[i].gene());

		++nfe;
		++nfe_local;

		if (nfe >= Max_NFE)
		{
			stop_flag = true;
			stop_idx = i;
			break;
		}
	}


	// DE Selection
	for (size_t i = 0; i < pop.size(); ++i)
	{
		if (stop_flag && i > stop_idx) break;

		if (children[i].fitness() <= pop[i].fitness())
		{
			if (children[i].fitness() < pop[i].fitness())
			{
				archive_pop.push_back(pop[i]);
				success_parameter.push_back(Memory(sample_parameter[i].F(), sample_parameter[i].CR()));
				success_fit_dif.push_back(std::fabs(children[i].fitness() - pop[i].fitness()));
			}
			pop[i] = children[i];
		}
	}
	pop.sort();
}

Individual CC_mL_SHADE::Solve(const Individual& context_vec, const Group& group, std::vector<std::vector<double>>& population, const CProblem& prob, Benchmarks* fp, const int iteration, unsigned long long int& nfe, unsigned long long int used_nfe, const size_t num_deps)
{
	// Set target problem 
	Individual::SetTargetProblem(prob);

	// Set parameter and random generator
	unsigned long long int nfe_local = 0;	// record the nfe number in this optimizer
	const size_t Gene_Len = prob.dim(), Nmin = nmin_, H = h_;
	size_t NP_first = NP_;

	const unsigned long long int Max_NFE = max_nfe_;
	int cur_iteration = 0;

	//const double pbest = p_, ub = prob.upper_bound(), lb = prob.lower_bound(), scalemax = scalemax_;
	//std::default_random_engine generator;
	
	// Set multiple mutatiion operator
	//std::string mut_opt = "Cur-to-pbest";
	enum Mutation_opt
	{	Cur_to_pbest,
		Cur_to_best,
		Cur_to_grbest
	};
	vector<string> mut_opt = { "Cur-to-pbest","Cur_to_best","Cur_to_grbest" };
	const size_t multi_operator_num = multi_operator_num_;

	// Initialize and evaluate population in this stage
	Population pop(NP_first), archive_pop;	//children(NP_init_);
	for (size_t i = 0; i < NP_first; ++i)
	{
		pop[i] = context_vec;
	}
	for (size_t i = 0; i < population.size(); ++i)
	{
		for (size_t j = 0; j < population[i].size(); ++j)
		{
			pop[i].gene()[group[j]] = population[i][j];
		}
	}
	for (size_t i = 0; i < NP_first; ++i)
	{
		pop[i].fitness() = fp->compute(pop[i].gene());
		++nfe;
		++nfe_local;
	}
	pop.sort();

	if (nfe >= Max_NFE)
	{
		return pop[0];
	}
		
	// Initialize memory sample system
	std::vector<Memory> sample_parameter, success_parameter;
	std::vector<double> success_fit_dif;
	

	// Set variables for terminal criteria
	//bool stop_flag = false;
	//size_t stop_idx = 0, 
	size_t no_success_cnt = 0, max_no_success_cnt = 0, perturb_cnt = 0, total_no_success = 0;


	while (cur_iteration < iteration && nfe < Max_NFE)
	{
		cur_iteration += 1;

		sample_parameter.clear();

		if (!success_parameter.empty())
		{
			success_parameter.clear();
			success_fit_dif.clear();
		}

		//Initialize each multi-operator population
		//Population pop_cp(NP_op_[0]), pop_cg(NP_op_[1]), pop_r2(NP_op_[2]);
		vector<Population> pop_op(multi_operator_num);
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			pop_op[i] = Population(NP_op_[i]);
		}
		pop.shuffle();
		for (size_t i = 0; i < NP_; ++i)
		{
			if (i < NP_op_[0])
				pop_op[0][i] = pop[i];
			else if (i < (NP_op_[0] + NP_op_[1]))
				pop_op[1][i - NP_op_[0]] = pop[i];
			else
				pop_op[2][i - (NP_op_[0] + NP_op_[1])] = pop[i];
		}
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			pop_op[i].sort();
		}



		// For each subcomponts in current popultaion
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			DEStrategy(pop, pop_op[i], archive_pop, sample_parameter, success_parameter, success_fit_dif, fp, nfe, nfe_local, mut_opt[i], prob);
		}
		

		for (size_t i = 0; i < NP_; ++i)
		{
			if (i < NP_op_[0])
				pop[i] = pop_op[0][i];
			else if (i < (NP_op_[0] + NP_op_[1]))
				pop[i] = pop_op[1][i - NP_op_[0]];
			else
				pop[i] = pop_op[2][i - (NP_op_[0] + NP_op_[1])];
		}
		pop.sort();

		// Resize archive
		if (archive_pop.size() > A_)
		{
			archive_pop.shuffle();
			archive_pop.resize(A_);
		}

		// Using the success individuals' F and CR to calculate the new lehmer mean
		if (success_parameter.size() > 0)
		{
			// Update success history memory
			memory_sys_.update_memory(success_parameter, success_fit_dif);
			total_no_success += no_success_cnt;
			no_success_cnt = 0;
		}
		else
		{
			no_success_cnt += NP_;
			if (max_no_success_cnt < no_success_cnt) max_no_success_cnt = no_success_cnt;

			// Memory pertubation
			if (alg_math::randDouble(0.0, 1.0) <= no_success_cnt * 1.0 / nfe)
			{
				memory_sys_.pertub_memory();
				++perturb_cnt;
				// Reset the counter
				total_no_success += no_success_cnt;
				no_success_cnt = 0;
			}
		}

		//calculate Diversity D_op
		vector<double> Diversity(multi_operator_num, 0);
		double D_sum = 0;
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			for (size_t j = 1; j < pop_op[i].size(); j += 1)
			{
				Diversity[i] += EuclideanDistance(pop_op[i][0], pop_op[i][j]);
			}
			Diversity[i] = Diversity[i] / pop_op[i].size();
			D_sum += Diversity[i];
		}
		//double D_cp = 0, D_cg = 0, D_r2 = 0;

		//calculate Diversity Rate
		vector<double> DR(multi_operator_num, 0);
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			DR[i] = Diversity[i] / D_sum;
		}

		//calculate solution quality
		vector<double> QR(multi_operator_num, 0);
		double best_fitness_sum = 0;
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			best_fitness_sum += pop_op[i][0].fitness();
		}
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			QR[i] = pop_op[i][0].fitness() / best_fitness_sum;
		}

		//calcualte improvement rate
		vector<double> IRV(multi_operator_num, 0);
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			IRV[i] = (1 - QR[i]) + DR[i];
		}

		// LPSR, Linear Population Size Reduction
		if (group.name() == "separable")
		{
			NP_ = (size_t)round(((1.0 * (int)(Nmin - NP_init_) / (double)(Max_NFE / num_deps)) * (nfe_local + used_nfe)) + NP_init_);
			A_ = (size_t)NP_ * rarc_;
		}
		if (NP_ < Nmin) NP_ = Nmin;

		double IRV_sum = 0;
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			IRV_sum += IRV[i];
		}

		size_t cur_NP_sum = 0;
		for (int i = 0; i < multi_operator_num; i += 1)
		{
			if (i != multi_operator_num - 1)
			{
				NP_op_[i] = (size_t)(max(0.1, min(0.9, IRV[i] / IRV_sum)) * NP_);
				cur_NP_sum += NP_op_[i];
			}
			else
			{
				if (cur_NP_sum >= NP_)
					NP_op_[i] = 0;
				else
					NP_op_[i] = NP_ - cur_NP_sum;
			}
		}
			

		pop.sort();
		pop.resize(NP_);
		if (archive_pop.size() > A_)
		{
			archive_pop.shuffle();
			archive_pop.resize(A_);
		}

		// END one iteration

	}
	pop.sort();

	// return the evolved population back to subcomponent's population
	population.resize(NP_);
	for (size_t i = 0; i < population.size(); ++i)
	{
		for (size_t j = 0; j < population[i].size(); ++j)
		{
			population[i][j] = pop[i].gene()[group[j]];
		}
	}
	
	// return new context vector
	return pop[0];
}


Individual::GeneVec CC_mL_SHADE::CurtopBest_DonorVec(int target_idx, double p, double f, const Population& pop, const Population& archive)
{
	size_t Pop_Size = pop.size();
	int best_idx_max = round(Pop_Size * p);
	if (best_idx_max < 2) best_idx_max = 2;

	int x_best = alg_math::randInt(0, best_idx_max - 1),
		x_r1 = alg_math::randInt(0, (int)(Pop_Size - 1)),
		x_r2 = alg_math::randInt(0, (int)(Pop_Size + archive.size() - 1));
	while (x_r1 == target_idx || x_r2 == target_idx || x_r1 == x_r2)
	{
		x_r1 = rand() % Pop_Size;
		x_r2 = rand() % (Pop_Size + archive.size());
	}
	size_t gene_len = pop[target_idx].gene().size();
	Individual::GeneVec g_cur = pop[target_idx].gene(),
		g_best = pop[x_best].gene(),
		g_r1 = pop[x_r1].gene(),
		g_r2 = (x_r2 >= Pop_Size) ? archive[x_r2 - Pop_Size].gene() : pop[x_r2].gene();

	Individual::GeneVec donor_vector(gene_len);
	for (size_t j = 0; j < gene_len; ++j)
	{
		donor_vector[j] = g_cur[j] + f * (g_best[j] - g_cur[j] + g_r1[j] - g_r2[j]);
	}
	return donor_vector;
}

Individual::GeneVec CC_mL_SHADE::CurtoBest_DonorVec(int target_idx, double f, const Population& pop)
{
	size_t Pop_Size = pop.size();
	//int best_idx_max = 0;
	//if (best_idx_max < 2) best_idx_max = 2;

	int x_best = 0,
		x_r1 = alg_math::randInt(0, (int)(Pop_Size - 1)),
		x_r2 = alg_math::randInt(0, (int)(Pop_Size - 1));
	while (x_r1 == target_idx || x_r2 == target_idx || x_r1 == x_r2)
	{
		x_r1 = rand() % Pop_Size;
		x_r2 = rand() % Pop_Size;
	}
	size_t gene_len = pop[target_idx].gene().size();
	Individual::GeneVec g_cur = pop[target_idx].gene(),
		g_best = pop[x_best].gene(),
		g_r1 = pop[x_r1].gene(),
		g_r2 = pop[x_r2].gene();

	Individual::GeneVec donor_vector(gene_len);
	for (size_t j = 0; j < gene_len; ++j)
	{
		donor_vector[j] = g_cur[j] + f * (g_best[j] - g_cur[j] + g_r1[j] - g_r2[j]);
	}
	return donor_vector;
}

Individual::GeneVec CC_mL_SHADE::Rand2_DonorVec(double f, const Population& pop)
{
	size_t Pop_Size = pop.size();

	std::vector<int> shuffle_vec(Pop_Size, 0);
	for (int i = 0; i < Pop_Size; ++i)
	{
		shuffle_vec[i] = i;
	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(shuffle_vec.begin(), shuffle_vec.end(), std::default_random_engine(seed));

	int x_r1 = shuffle_vec[0],
		x_r2 = shuffle_vec[1],
		x_r3 = shuffle_vec[2],
		x_r4 = shuffle_vec[3],
		x_r5 = shuffle_vec[4];

	size_t gene_len = pop[x_r1].gene().size();
	Individual::GeneVec g_r1 = pop[x_r1].gene(),
		g_r2 = pop[x_r2].gene(),
		g_r3 = pop[x_r3].gene(),
		g_r4 = pop[x_r4].gene(),
		g_r5 = pop[x_r5].gene();

	Individual::GeneVec donor_vector(gene_len);
	for (size_t j = 0; j < gene_len; ++j)
	{
		donor_vector[j] = g_r1[j] + f * (g_r2[j] - g_r3[j]) + f * (g_r4[j] - g_r5[j]);
	}
	return donor_vector;
}

Individual::GeneVec CC_mL_SHADE::CurtogrBest_DonorVec(int target_idx, double p, double f, const Population& pop)
{
	size_t Pop_Size = pop.size();
	int p_pop_size = round(Pop_Size * p);
	//if (best_idx_max < 2) best_idx_max = 2;

	std::vector<int> shuffle_vec(Pop_Size, 0);
	for (int i = 0; i < Pop_Size; i += 1)
	{
		shuffle_vec[i] = i;
	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(shuffle_vec.begin(), shuffle_vec.end(), std::default_random_engine(seed));

	int x_grbest = p_pop_size;
	for (int i = 0; i < p_pop_size; ++i)
	{
		if (shuffle_vec[i] != target_idx)
			x_grbest = std::min(x_grbest, shuffle_vec[i]);
	}

	std::shuffle(shuffle_vec.begin(), shuffle_vec.end(), std::default_random_engine(seed));
	int	x_r1 = 0;
	for (int i = 0; i < Pop_Size; ++i)
	{
		if (shuffle_vec[i] != x_grbest && shuffle_vec[i] != target_idx)
		{
			x_r1 = shuffle_vec[i];
			break;
		}
	}

	int x_r2 = 0;
	for (int i = 0; i < Pop_Size; ++i)
	{
		if (shuffle_vec[i] != x_grbest && shuffle_vec[i] != target_idx && shuffle_vec[i] != x_r1)
		{
			x_r2 = shuffle_vec[i];
			break;
		}
	}

	size_t gene_len = pop[target_idx].gene().size();
	Individual::GeneVec g_cur = pop[target_idx].gene(),
		g_grbest = pop[x_grbest].gene(),
		g_r1 = pop[x_r1].gene(),
		g_r2 = pop[x_r2].gene();

	Individual::GeneVec donor_vector(gene_len);
	for (size_t j = 0; j < gene_len; ++j)
	{
		donor_vector[j] = g_cur[j] + f * (g_grbest[j] - g_cur[j] + g_r1[j] - g_r2[j]);
	}
	return donor_vector;
}