#include "alg_result.h"
#include "self_implement_functions.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <filesystem>

Result::Result(int ID, int num_run, string fileName)
{
	ID_ = ID;
	num_of_Run_ = num_run;
	best_fitness_vec_ = vector<double>(num_run, 0.0);

	string dirName = "output/";
	if (!(filesystem::exists(dirName))) {
		filesystem::create_directory(dirName);
	}
	
	fresult_ = fstream(dirName + fileName, ios::out | ios::app);
	f_all_result_ = fstream(dirName + IntToStr(num_of_Run_, 2) + "run_" + fileName, ios::out | ios::app);
}

void Result::compute(const int global_optimum)
{
	sort(best_fitness_vec_.begin(), best_fitness_vec_.end());

	fitness_best_ = best_fitness_vec_.front();
	fitness_worst_ = best_fitness_vec_.back();
	fitness_median_ = (best_fitness_vec_[num_of_Run_ / 2] + best_fitness_vec_[num_of_Run_ / 2 - 1]) / 2;

	double sum = 0.0;
	for (size_t i = 0; i < num_of_Run_; ++i)
	{
		sum += best_fitness_vec_[i];
	}
	fitness_mean_ = sum / num_of_Run_;

	sum = 0.0;
	for (size_t i = 0; i < num_of_Run_; ++i)
	{
		sum += pow((best_fitness_vec_[i] - fitness_mean_), 2);
	}
	fitness_std_ = sqrt(sum / num_of_Run_);
	
	error_best_ = fitness_best_ - global_optimum;
	if (error_best_ <= pow(10, -8))		error_best_ = 0;
	error_worst_ = fitness_worst_ - global_optimum;
	if (error_worst_ <= pow(10, -8))	error_worst_ = 0;
	error_median_ = fitness_median_ - global_optimum;
	if (error_median_ <= pow(10, -8))	error_median_ = 0;
	error_mean_ = fitness_mean_ - global_optimum;
	if (error_mean_ <= pow(10, -8))		error_mean_ = 0;

	sum = 0.0;
	for (size_t i = 0; i < num_of_Run_; ++i)
	{
		double error_tmp = best_fitness_vec_[i] - global_optimum;
		if (error_tmp <= pow(10, -8))	error_tmp = 0;

		sum += pow((error_tmp - error_mean_), 2);
	}
	error_std_ = sqrt(sum / num_of_Run_);

	cout << "Fitness     Best: " << fitness_best_ << ", worst: " << fitness_worst_ << ", median: " << fitness_median_ << ", mean: " << fitness_mean_ << ", std: " << fitness_std_ << endl;
	cout << "ErrorValue  Best: " << error_best_ << ", worst: " << error_worst_ << ", median: " << error_median_ << ", mean: " << error_mean_ << ", std: " << error_std_ << endl;

}

void Result::outputToFile(const int global_optimum)
{
	// result
	fresult_ << ID_ << ",";
	fresult_ << setprecision(16) << error_best_ << ",";
	fresult_ << setprecision(16) << error_worst_ << ",";
	fresult_ << setprecision(16) << error_median_ << ",";
	fresult_ << setprecision(16) << error_mean_ << ",";
	fresult_ << setprecision(16) << error_std_ << ",";
	fresult_ << endl;

	// all runs fitness error values after sorting
	for (int i = 0; i<best_fitness_vec_.size(); ++i)
	{
		double tmp = best_fitness_vec_[i] - global_optimum;
		if (tmp < pow(10, -8))
			tmp = 0;

		f_all_result_ << setprecision(16) << tmp << endl;
	}
}

void Result::close()
{
	fresult_.close();
	f_all_result_.close();
}