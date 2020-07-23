#ifndef RESULT__
#define RESULT__

#include "alg_result.h";
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Result
{
public:
	explicit Result(int ID, int num_run, string fileName);

	double& operator[](size_t i) { return best_fitness_vec_[i]; }
	const double& operator[](size_t i) const { return best_fitness_vec_[i]; }
	const double fitness_best() const { return fitness_best_; }
	const double fitness_worst() const { return fitness_worst_; }
	const double fitness_median() const { return fitness_median_; }
	const double fitness_mean() const { return fitness_mean_; }
	const double fitness_std() const { return fitness_std_; }
	const double error_best() const { return error_best_; }
	const double error_worst() const { return error_worst_; }
	const double error_median() const { return error_median_; }
	const double error_mean() const { return error_mean_; }
	const double error_std() const { return error_std_; }

	void compute(const int global_optimum);
	void outputToFile(const int global_optimum);
	void close();
private:
	vector<double> best_fitness_vec_;
	int num_of_Run_;
	int ID_;

	fstream fresult_;
	fstream f_all_result_;

	double fitness_best_, fitness_worst_, fitness_median_, fitness_mean_, fitness_std_;
	double error_best_, error_worst_, error_median_, error_mean_, error_std_;

};


#endif // !RESULT__

