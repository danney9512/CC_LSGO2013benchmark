#ifndef CC_OPTIMIZER_MLSHADE__
#define CC_OPTIMIZER_MLSHADE__

#include "alg_individual.h"
#include "alg_population.h"
#include "alg_base.h"
#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

#include "2013LSGOBenchmarks/2013Benchmarks.h"

class Group;

class CC_mL_SHADE
{
public:
	CC_mL_SHADE() {}
	explicit CC_mL_SHADE(const size_t NP, const unsigned long long int maxFE);

	const std::string& name() const { return name_; }

	void Setup(std::ifstream& ifile);
	Individual Solve(const Individual& context_vec, const Group& group, std::vector<std::vector<double>>& population, const CProblem& prob, Benchmarks* fp, const int iteration, unsigned long long int& nfe, unsigned long long int used_nfe, const size_t num_deps);

	class Memory
	{
	public:
		explicit Memory(double f = 0.0, double cr = 0.0) : f_(f), cr_(cr) {}

		double& F() { return f_; }
		const double& F() const { return f_; }
		double& CR() { return cr_; }
		const double& CR() const { return cr_; }

	private:
		double f_,
			cr_;
	};
	class MemorySystem
	{
	public:
		explicit MemorySystem(std::size_t H = 0) : memories_(H), k_(0), h_(H) {}

		size_t& K() { return k_; }
		const size_t& K() const { return k_; }
		size_t& H() { return h_; }
		const size_t& H() const { return h_; }
		Memory& operator[](std::size_t i) { return memories_[i]; }
		const Memory& operator[](std::size_t i) const { return memories_[i]; }

		std::size_t size() const { return memories_.size(); }
		bool empty() const { return size() == 0; }
		void push_back(const Memory& mem)
		{
			memories_.push_back(mem);
			h_ += 1;
		}
		void resize(std::size_t t) { memories_.resize(t); }
		void update_memory(const std::vector<Memory>& success_parameter, const std::vector<double>& success_fit_dif);
		void pertub_memory();
	private:
		size_t k_, h_;
		std::vector<Memory> memories_;
	};

	void DEStrategy(const Population& bigpop, Population& pop, Population& archive_pop, std::vector<Memory>& sample_parameter, std::vector<Memory>& success_parameter, std::vector<double>& success_fit_dif, Benchmarks* fp, unsigned long long int& nfe, unsigned long long int& nfe_local, const string& mut_opt, const CProblem& prob);

private:
	double p_,
		rarc_,
		finit_,
		crinit_,
		scalemax_;
	size_t h_,
		rNinit_,
		nmin_;
	std::string name_{};
	unsigned long long int max_nfe_;

	MemorySystem memory_sys_;

	size_t NP_init_, 
		NP_,
		A_;

	size_t multi_operator_num_;
	std::vector<size_t> NP_op_;
	
	Individual::GeneVec CurtopBest_DonorVec(int target_idx, double p, double f, const Population& pop, const Population& archive);
	Individual::GeneVec Rand2_DonorVec(double f, const Population& pop);
	Individual::GeneVec CurtogrBest_DonorVec(int target_idx, double p, double f, const Population& pop);
	Individual::GeneVec CurtoBest_DonorVec(int target_idx, double f, const Population& pop);
};



#endif // !CC_ML_SHADE__

