#include "alg_log.h"
#include <cmath>
#include <iomanip>
#include <filesystem>

namespace filesystem = std::filesystem;

const int get_score(Individual* indiv, const double& optimum)
{
	double fitness = indiv->fitness();
	for (int i = 0; i <= 9; ++i)
	{
		if (fitness - optimum >= pow(10.0, -i))
		{
			return i;
		}
	}
	return 10;
}

int Log::numberOfk = 0;

Log::Log(string fileName)
{
	now_score = 0;
	if (!(filesystem::exists("output/"))) {
		filesystem::create_directory("output/");
		filesystem::copy("python/", "output/");
	}
	string dirName = "output/" + fileName;
	if (!(filesystem::exists(dirName))) {
		filesystem::create_directory(dirName);
	}
	string dirK = dirName + "/" + to_string(Log::numberOfk);
	if (!(filesystem::exists(dirK))) {
		filesystem::create_directory(dirK);
	}
	fitness_stream_ = fstream(dirK + "/best_fitness.csv", ios::out);
	parameter_stream_ = fstream(dirK + "/parameter.txt", ios::out);

	if (numberOfk != 0)
	{
		score_stream_ = fstream(dirName + "/score.csv", ios::out | ios::app);
	}
	else
	{
		score_stream_ = fstream(dirName + "/score.csv", ios::out | ios::trunc);
		score_stream_ << endl;
	}
	score_ffe_vector = vector<int>(10, 0);
}

Log::Log(string fileName, int max_FFE, int dim, int optimum)
{
	max_FFE_ = max_FFE;
	problem_dim_ = dim;
	global_optimum_ = optimum;

	counter_k_ = 0;
	output_FFE_ = 0;

	//now_score = 0;

	if (!(filesystem::exists("output/"))) {
		filesystem::create_directory("output/");
		filesystem::copy("python/", "output/");
	}
	string dirName = "output/" + fileName;
	if (!(filesystem::exists(dirName))) {
		filesystem::create_directory(dirName);
	}
	string dirK = dirName + "/" + to_string(Log::numberOfk);
	if (!(filesystem::exists(dirK))) {
		filesystem::create_directory(dirK);
	}

	fitness_stream_ = fstream(dirK + "/best_fitness.csv", ios::out);
	
	errorvalue_stream_ = fstream(dirK + "/ErrorValue.csv", ios::out);

	//population_stream_ = fstream(dirK + "/population.txt", ios::out);

	//archive_stream_ = fstream(dirK + "/archive.txt", ios::out);
	//parameter_stream_ = fstream(dirK + "/parameter.txt", ios::out);
}

bool Log::LSGO2013_record_point(const int nfe)
{
	int record_point_arr[3] = {	120000, 600000, 3000000 };

	int output_FFE = record_point_arr[counter_k_];
	if (nfe >= output_FFE)
	{
		output_FFE_ = output_FFE;
		counter_k_ += 1;
		return true;
	}
	else
	{
		return false;
	}
}

bool Log::CEC2020_record_point(const int nfe)
{
	// how many monement need to be recorded
	constexpr int num_k = 15;

	// for CEC2020 Bound Constrained Benchmark
	double output_FFE = floor(pow(problem_dim_, (counter_k_ / 5.0 - 3.0)) * max_FFE_);
	if (nfe >= output_FFE)
	{
		output_FFE_ = output_FFE;
		counter_k_ += 1;
		return true;
	}
	else
	{
		return false;
	}
}


void Log::store_pop(Population* pop, const int nfe)
{
	// Output population
	Population& cpop = *pop;
	size_t popSize = pop->size();

	population_stream_ << "nfe: " << nfe << ", Population size: " << popSize << endl;
	for (size_t i = 0; i < popSize; ++i)
	{
		population_stream_ << "pop " << i << ": fitness = " << setprecision(8) << cpop[i].fitness() << ", gene = ";
		for (size_t j = 0; j < cpop[i].gene().size(); ++j)
		{
			population_stream_ << setprecision(8) << cpop[i].gene()[j] << " ";
		}
		population_stream_ << endl;
	}
	population_stream_ << endl;

}

void Log::store_subpop(Population* subpop, const int subpop_num)
{
	// Output sub-population
	Population& cpop = *subpop;
	size_t popSize = subpop->size();

	population_stream_ << "Sub-pop " << subpop_num << endl;
	population_stream_ << "nfe: " << output_FFE_ << ", Population size: " << popSize << endl;
	for (size_t i = 0; i < popSize; ++i)
	{
		population_stream_ << "pop " << i << ": fitness = " << setprecision(8) << cpop[i].fitness() << ", gene = ";
		for (size_t j = 0; j < cpop[i].gene().size(); ++j)
		{
			population_stream_ << setprecision(8) << cpop[i].gene()[j] << " ";
		}
		population_stream_ << endl;
	}
	population_stream_ << endl;
}

void Log::store_archive(Population* archive_pop)
{
	// Output archive population

	Population& Apop = *archive_pop;
	size_t ApopSize = archive_pop->size();

	archive_stream_ << "nfe: " << output_FFE_ << ", Archive population size: " << ApopSize << endl;
	for (size_t i = 0; i < ApopSize; ++i)
	{
		archive_stream_ << "A " << i << ": fitness = " << Apop[i].fitness() << ", gene = ";
		for (size_t j = 0; j < Apop[i].gene().size(); ++j)
		{
			archive_stream_ << Apop[i].gene()[j] << " ";
		}
		archive_stream_ << endl;
	}
	archive_stream_ << endl;

}

void Log::store_bestfitness(Population* pop, const int nfe)
{
	Population& cpop = *pop;

	// store best fitness
	fitness_stream_ << nfe << "," << setprecision(16) << cpop[0].fitness() << endl;
}


void Log::store_errorvalue(Population* pop)
{
	Population& cpop = *pop;

	// Error value after FFE
	errorvalue_stream_ << output_FFE_ << ",";

	double error_value = cpop[0].fitness() - global_optimum_;
	if (error_value <= pow(10, -8))
		errorvalue_stream_ << setprecision(16) << 0 << endl;
	else
		errorvalue_stream_ << setprecision(16) << error_value << endl;
}

void Log::store(Population* pop, const mL_SHADE::MemorySystem& mry, const std::vector<mL_SHADE::Memory>& sample_parameter, const std::vector<mL_SHADE::Memory>& success_parameter, const int nfe)
{
	Population& cpop = *pop;

	/*
	// Find best
	size_t popSize = pop->size();
	int x_best = -1;
	double fitness_best = -1.0;
	for (size_t i = 0; i < popSize; ++i)
	{
		if (cpop[i].fitness() < fitness_best || fitness_best == -1.0)
		{
			fitness_best = cpop[i].fitness();
			x_best = (int)(i);
		}
	}
	// store best fitness
	fitness_stream_ << nfe << "," << setprecision(16) << cpop[x_best].fitness() << endl;

	//update score

	const int newScore = get_score(&(cpop[x_best]), 1.0);
	if (newScore > now_score)
	{
		score_stream_ << numberOfk << ","
			<< nfe << ","
			<< setprecision(16) << cpop[x_best].fitness() << ","
			<< newScore << "," << endl;

		while (now_score <= newScore)
		{
			if (now_score != 0) score_ffe_vector[now_score - 1] = nfe;
			now_score++;
		}
		now_score = newScore;
	}
	*/

	// Record parameter
	parameter_stream_ << nfe << endl;
	parameter_stream_ << "m.f: ";
	for (size_t i = 0; i < mry.size(); ++i) parameter_stream_ << mry[i].F() << " ";
	parameter_stream_ << endl << "m.cr: ";
	for (size_t i = 0; i < mry.size(); ++i) parameter_stream_ << mry[i].CR() << " ";
	parameter_stream_ << endl << "sa.f: ";
	for (size_t i = 0; i < sample_parameter.size(); ++i) parameter_stream_ << sample_parameter[i].F() << " ";
	parameter_stream_ << endl << "sa.cr: ";
	for (size_t i = 0; i < sample_parameter.size(); ++i) parameter_stream_ << sample_parameter[i].CR() << " ";
	parameter_stream_ << endl << "su.f: ";
	for (size_t i = 0; i < success_parameter.size(); ++i) parameter_stream_ << success_parameter[i].F() << " ";
	parameter_stream_ << endl << "su.cr: ";
	for (size_t i = 0; i < success_parameter.size(); ++i) parameter_stream_ << success_parameter[i].CR() << " ";
	parameter_stream_ << endl;
}

void Log::store(Population* pop, const L_SHADE::MemorySystem& mry, const std::vector<L_SHADE::Memory>& sample_parameter, const std::vector<L_SHADE::Memory>& success_parameter, const int nfe)
{
	Population& cpop = *pop;

	//find best
	/*
	size_t popSize = pop->size();
	int x_best = -1;
	double fitness_best = -1.0;
	for (size_t i = 0; i < popSize; ++i)
	{
		if (cpop[i].fitness() < fitness_best || fitness_best == -1.0)
		{
			fitness_best = cpop[i].fitness();
			x_best = (int)(i);
		}
	}
	//get best fitness
	fitness_stream_ << nfe << "," << setprecision(16) << cpop[x_best].fitness() << endl;

	//update score

	const int newScore = get_score(&(cpop[x_best]), 1.0);
	if (newScore > now_score)
	{
		score_stream_ << numberOfk << ","
			<< nfe << ","
			<< setprecision(16) << cpop[x_best].fitness() << ","
			<< newScore << "," << endl;

		while (now_score <= newScore)
		{
			if (now_score != 0) score_ffe_vector[now_score - 1] = nfe;
			now_score++;
		}
		now_score = newScore;
	}
	*/

	// Record parameter
	parameter_stream_ << nfe << endl;
	parameter_stream_ << "m.f: ";
	for (size_t i = 0; i < mry.size(); ++i) parameter_stream_ << mry[i].F() << " ";
	parameter_stream_ << endl << "m.cr: ";
	for (size_t i = 0; i < mry.size(); ++i) parameter_stream_ << mry[i].CR() << " ";
	parameter_stream_ << endl << "sa.f: ";
	for (size_t i = 0; i < sample_parameter.size(); ++i) parameter_stream_ << sample_parameter[i].F() << " ";
	parameter_stream_ << endl << "sa.cr: ";
	for (size_t i = 0; i < sample_parameter.size(); ++i) parameter_stream_ << sample_parameter[i].CR() << " ";
	parameter_stream_ << endl << "su.f: ";
	for (size_t i = 0; i < success_parameter.size(); ++i) parameter_stream_ << success_parameter[i].F() << " ";
	parameter_stream_ << endl << "su.cr: ";
	for (size_t i = 0; i < success_parameter.size(); ++i) parameter_stream_ << success_parameter[i].CR() << " ";
	parameter_stream_ << endl;
}


void Log::close() {
	//score_stream_.close();
	fitness_stream_.close();
	parameter_stream_.close();
	errorvalue_stream_.close();

	population_stream_.close();
	//archive_stream_.close();
}