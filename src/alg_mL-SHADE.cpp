#include "alg_mL-SHADE.h"
#include "alg_population.h"
#include "alg_initialization.h"
#include "alg_mutation.h"
#include "alg_crossover.h"
#include "alg_math.h"
#include "problem.h"
#include "alg_log.h"
#include "CC_group.h"

#include "2013LSGOBenchmarks/2013Benchmarks.h"

#include <string>
#include <cstdlib>
#include <cmath>
#include <random>
#include <iostream>
#include <climits>
#include <vector>
using namespace std;

//-------------------------------
// mL-SHADE
//-------------------------------
void mL_SHADE::Setup(std::ifstream &ifile)
{
	if (!ifile) return;

	// Loading and Setting
	string dummy;
	ifile >> dummy >> dummy >> name_;
	ifile >> dummy >> dummy >> rNinit_;
	ifile >> dummy >> dummy >> nmin_;
	//ifile >> dummy >> dummy >> max_nfe_;	// 已被拿出去
	ifile >> dummy >> dummy >> finit_;
	ifile >> dummy >> dummy >> crinit_;
	ifile >> dummy >> dummy >> rarc_;
	ifile >> dummy >> dummy >> p_;
	ifile >> dummy >> dummy >> h_;
	ifile >> dummy >> dummy >> scalemax_;
}

void mL_SHADE::MemorySystem::update_memory(const std::vector<Memory> &success_parameter, const std::vector<double> &success_fit_dif)
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

void mL_SHADE::Solve(Individual& context_vec, const Group& group, const vector<vector<double>>& population, const CProblem &prob, const int iteration, const int pop_size, unsigned long long int& nfe, const unsigned long long int max_nFE)
{
	// Set log system and target problem 
	//Log log(name() + '_' + to_string(prob.id()) + "_D" + to_string(prob.dim()) + "_" + prob.name(), max_nfe_, prob.dim(), prob.global_optimum());
	Individual::SetTargetProblem(prob);
	
	// Set parameter and random generator
	//const size_t NPinit = (size_t) round(prob.dim() * rNinit_), Gene_Len = prob.dim(), H = h_, Nmin = nmin_;
	const size_t NPinit = (size_t)pop_size, Gene_Len = prob.dim(), H = h_, Nmin = nmin_;
	const unsigned long long int Max_NFE = max_nFE;
	int cur_iteration = 0;
	//unsigned long long int nfe = 0;
	size_t NP = NPinit, A = (size_t) (NPinit*rarc_);
	const double pbest = p_, ub = prob.upper_bound(), lb = prob.lower_bound(), scalemax = scalemax_;
	std::default_random_engine generator;

	// Create Benchmark function
	Benchmarks* fp = generateFuncObj(prob.id(), prob.lower_bound(), prob.upper_bound(), prob.dim());
	fp->nextRun();

	// Initialize and evaluate population  
	Population pop(NP), archive_pop, children(NP);
	//RandomInitialization(&pop, prob);
	for (int i = 0; i < NP; i += 1)
	{
		pop[i] = context_vec;
	}
	for (int i = 0; i < population.size(); i += 1)
	{
		for (int j = 0; j < population[i].size(); j += 1)
		{
			pop[i].gene()[group[j]] = population[i][j];
		}
	}
	for (size_t i = 0; i < NP; ++i)
	{
		pop[i].fitness() = fp->compute(pop[i].gene());
		++nfe;
	}
	pop.sort();

	// Initialize memory system
	MemorySystem memory_sys(H);
	for (size_t i = 0; i < H; ++i) memory_sys[i] = Memory(finit_, crinit_);
	std::vector<Memory> sample_parameter, success_parameter;
	std::vector<double> success_fit_dif;

	// Set variables for terminal criteria
	bool stop_flag = false;
	size_t stop_idx = 0, no_success_cnt = 0, max_no_success_cnt = 0, total_no_success = 0;

	//log.store(&pop, memory_sys, sample_parameter, success_parameter, (int)nfe);

	constexpr int bestFitness_log_cycle = 20;
	//size_t cycle_cnt = 0;
	
	while (cur_iteration < iteration && nfe < Max_NFE)
	{
		//cycle_cnt += 1;
		cur_iteration += 1;

		sample_parameter.clear();
		
		if (!success_parameter.empty())
		{
			success_parameter.clear();
			success_fit_dif.clear();
		}

		// For each individual in current popultaion
		//for (size_t i = 0; i < NP; ++i)
		for(size_t i = 0; i < group.size(); i += 1)
		{
			// Generate CRi and Fi
			int r = alg_math::randInt(0, (int)(H - 1));
			
			double CRi;
			if (memory_sys[r].CR() == -1.0)	CRi = 0.0;
			else
			{
				std::normal_distribution<double> randn(memory_sys[r].CR(), 0.1);
				CRi = std::min(1.0, std::max(0.0, randn(generator)));
			}

			std::cauchy_distribution<double> randc(memory_sys[r].F(), 0.1);
			double Fi = randc(generator);
			while (Fi <= 0.0) Fi = randc(generator);
			if (Fi > 1.0) Fi = 1.0;

			sample_parameter.push_back(Memory(Fi, CRi));

			// cur-to-pbest mutation
			Individual::GeneVec donor_vector;
			//donor_vector = CurtopBest_DonorVec((int)i, pbest, Fi, pop, archive_pop);
			donor_vector = CurtopBest_DonorVec(group[i], pbest, Fi, pop, archive_pop);

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

			if (nfe >= Max_NFE)
			{
				stop_flag = true;
				stop_idx = i;

				//log.store_errorvalue(&pop);
				//log.store_bestfitness(&pop, (int)nfe);
				break;
			}
		}

		// DE Selection
		for (size_t i = 0; i < NP; ++i)
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
		// Resize archive 
		if (archive_pop.size() > A)
		{
			archive_pop.shuffle();
			archive_pop.resize(A);
		}
		// Using the success individuals' F and CR to calculate the new lehmer mean
		if (success_parameter.size() > 0)
		{
			memory_sys.update_memory(success_parameter, success_fit_dif);
			total_no_success += no_success_cnt;
			no_success_cnt = 0;
		}
		else
		{
			no_success_cnt += NP;
			if (max_no_success_cnt < no_success_cnt) max_no_success_cnt = no_success_cnt;
		}	

		// LPSR
		NP = (size_t)round( ( (1.0* (int)(Nmin - NPinit)  / (double)Max_NFE) * nfe) + NPinit);
		A = (size_t) NP * rarc_;

		pop.sort();
		pop.resize(NP);
		if (archive_pop.size() > A)
		{
			archive_pop.shuffle();
			archive_pop.resize(A);
		}


		// Log data output
		/*
		if (log.LSGO2013_record_point((int)nfe))
		{
			log.store_errorvalue(&pop);
		}
		if (cycle_cnt % bestFitness_log_cycle == 0 || nfe >= Max_NFE)
		{
			log.store_bestfitness(&pop, (int)nfe);
		}*/

		//log.store(&pop, memory_sys, sample_parameter, success_parameter, (int)nfe);
		//log.store_pop(&pop, &archive_pop, (int)nfe);


		// Terminate process when 10 points are found
		//if (CEC2019_100Digit_Score(&pop[0], prob.global_optimum()) >= 10) break;
	}
	pop.sort();
	
	//update context vector
	context_vec = pop[0];

	//std::cout << "MNSC : " << max_no_success_cnt << " / LNSC : " << no_success_cnt << " / TNSC : " << total_no_success <<" ";
	//*solutions = pop;
	//log.close();
}

Individual::GeneVec mL_SHADE::CurtopBest_DonorVec(int target_idx, double p, double f, const Population& pop, const Population& archive)
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

Individual::GeneVec mL_SHADE::Rand2_DonorVec(double f, const Population& pop)
{
	size_t Pop_Size = pop.size();

	std::vector<int> shuffle_vec(Pop_Size, 0);
	for (int i = 0; i < Pop_Size; i += 1)
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