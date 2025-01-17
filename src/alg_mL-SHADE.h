#ifndef ALG_mLSHADE__
#define ALG_mLSHADE__

#include "alg_individual.h"
#include "alg_population.h"
#include "alg_base.h"
#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

class Group;

class mL_SHADE : public BaseEA
{
public:
	mL_SHADE() : BaseEA("mL-SHADE") {}
	explicit mL_SHADE(unsigned long long int maxFE) : BaseEA("mL-SHADE") { max_nfe_ = maxFE; }

	virtual void Setup(std::ifstream& ifile);
	virtual void Solve(Population* solutions, const CProblem& prob) {};
	virtual void Solve(Individual& context_vec, const Group& group, const std::vector<std::vector<double>>& population, const CProblem& prob, const int iteration, const int pop_size, unsigned long long int& nfe, const unsigned long long int max_nFE);

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

private:
	double p_,
		rarc_,
		finit_,
		crinit_,
		scalemax_;
	size_t h_,
		rNinit_,
		nmin_;
	unsigned long long int max_nfe_;
	Individual::GeneVec CurtopBest_DonorVec(int target_idx, double p, double f, const Population& pop, const Population& archive);
	Individual::GeneVec Rand2_DonorVec(double f, const Population& pop);
	Individual::GeneVec CurtogrBest_DonorVec(int target_idx, double p, double f, const Population& pop);
};
#endif // !ALG_mLSHADE__


