#ifndef LOG__
#define LOG__

#include <string>
#include <vector>
#include <fstream>
#include "alg_individual.h"
#include "alg_population.h"
#include "problem.h"
#include "alg_mL-SHADE.h"
#include "alg_L-SHADE.h"

using namespace std;

class Log
{
public:
	explicit Log(string fileName);
	explicit Log(string fileName, int max_FFE, int dim, int optimum);
	void store(Population *pop, const mL_SHADE::MemorySystem &mry, const std::vector<mL_SHADE::Memory> & sample_parameter, const std::vector<mL_SHADE::Memory> & success_parameter, const int nfe);
	void store(Population* pop, const L_SHADE::MemorySystem& mry, const std::vector<L_SHADE::Memory>& sample_parameter, const std::vector<L_SHADE::Memory>& success_parameter, const int nfe);
	void close();
	static void set_k(int k) { numberOfk = k; }


	void store_pop(Population* pop, const int nfe);
	void store_bestfitness(Population* pop, const int nfe);

	bool LSGO2013_record_point(const int nfe);
	bool CEC2020_record_point(const int nfe);

	void store_subpop(Population* subpop, const int subpop_num);
	void store_archive(Population* archive_pop);
	void store_errorvalue(Population* pop);
	
	void store_bestfitness(const Individual& sol, const size_t group_index, const unsigned long long int nfe);
	void store_errorvalue(const Individual& sol);

private:
	static int numberOfk;

	// record data counter
	int counter_k_;
	int output_FFE_;

	fstream fitness_stream_;
	fstream parameter_stream_;

	// Error Value
	int max_FFE_;
	int problem_dim_;
	int global_optimum_;
	fstream errorvalue_stream_;

	// Population
	fstream population_stream_;
	fstream archive_stream_;

	// 100digit score
	fstream score_stream_;
	string outputFileName;
	int now_score;
	vector<int> score_ffe_vector;
	
};

#endif // !LOG__

