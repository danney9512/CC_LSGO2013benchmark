// mL-SHADE_LSGO2013benchmark.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
// std C++ 17 compiler

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <vector>

#include <omp.h>

#include "2013LSGOBenchmarks/2013Benchmarks.h"
#include "self_implement_functions.h"
#include "problem_set.h"
#include "experiment.h"

#include "CC.h"

#include "alg_base.h"
#include "alg_log.h"
#include "alg_population.h"
#include "alg_initialization.h"
#include "alg_mL-SHADE.h"
#include "alg_result.h"


using namespace std;

int main()
{
	// Set random seed
	// 設定隨機種子
	//srand(time(0));

	// Load problem and experiment setting //
	// 載入 問題函式列表
	ifstream prob_set_ini("1_problem_list.ini");
	if(!prob_set_ini) 
		cout << "Error msg: Can't load the problems' definition." << endl;
	CPromblemSet prob_set(prob_set_ini);
	
	// 載入 實驗設定
	ifstream exp_list_ini("2_experiment_list.ini");
	if(!exp_list_ini)
		cout << "Error msg: Can't load the experiments." << endl;
	ExperimentSet exp_set(exp_list_ini);

	// Define variables
	// 宣告實驗變數
	constexpr int NumOfRuns = 1;

	CC_alg* CC = nullptr;

	string algo_name, algo_ini_str, grouping_name, optimizer_name, optimizer_ini_str;
	size_t problem_id = 0;


	// Compute time initialize
	// 計算時間初始化
	double allExp_START, allExp_END;

	allExp_START = clock();
	// Run Experiment
	for (size_t exp_i = 0; exp_i < exp_set.size(); exp_i += 1)
	{
		algo_name = exp_set[exp_i].algo_name();
		problem_id = exp_set[exp_i].problem_id();
		optimizer_name = exp_set[exp_i].optimizer();
		grouping_name = exp_set[exp_i].grouping();
		bool grouped = exp_set[exp_i].grouped();

		cout << "Experiment" << exp_i + 1 << ", " << algo_name << " solves " << prob_set[problem_id - 1].name() << ": " << endl;
		
		GroupsResult groupingResult;

		if (algo_name == "CC")
		{
			// Grouping 
			if (grouped)
			{
				// 去讀分群好的檔案不用再分群了
				groupingResult.inputResult(grouping_name, problem_id);
			}
			else
			{
				// 執行分群演算法，將分群結果直接給 CC 建構
			}

			// Construct CC
			CC = new CC_alg(groupingResult.FFE_cost(), exp_set[exp_i].max_nFE());

			cout << exp_i << endl;

			/*
			cout << "Separable variables: " << endl;
			for (int i = 0; i < groupingResult.sep_groups().size(); ++i)
			{
				cout << groupingResult.sep_groups()[i][0] << " ";
			}
			cout << endl << endl;
			cout << "Non-Separable variables: " << endl;
			for (int i = 0; i < groupingResult.nonsep_groups().size(); ++i)
			{
				for (int j = 0; j < groupingResult.nonsep_groups()[i].size(); ++j)
				{
					cout << groupingResult.nonsep_groups()[i][j] << " ";
				}
				cout << endl;
			}
			*/

		}
		else
		{
			cout << "\tError msg: Can't recognize the algorithm." << endl;
			continue;
		}

		// Load optimizer 參數
		// Loading Algorithm parameter's setting
		// 載入演算法參數

		algo_ini_str = "experiments\\" + algo_name + "_parameter.ini";
		ifstream algo_para_ini(algo_ini_str);
		
		optimizer_ini_str  = "experiments\\optimizer\\" + optimizer_name + "\\exp_CEC2013benchmark_" + IntToStr(problem_id, 2) + ".ini";
		ifstream optimizer_para_ini(optimizer_ini_str);


		if (!algo_para_ini || !optimizer_para_ini)
		{
			if (!algo_para_ini)
			{
				cout << "\tError msg: Can't load the CC parameters, check the .ini file's path." << endl;
			}
			if (!optimizer_para_ini)
			{
				cout << "\tError msg: Can't load the CC optimizer's parameters, check the .ini file's path." << endl;
			}
			continue;
		}
		else
		{
			
			// Execution the Algorithm
			// 執行演算法

			// Computing time initialize for each experiment
			double START, END;

			// Algorithm parameter's setting
			CC -> Setup(algo_para_ini, groupingResult, optimizer_name, optimizer_ini_str);



			//string result_filename = algo_name + optimizer_name + "-" + grouping_name + "_F" + IntToStr(problem_id, 2) + "_D" + IntToStr(prob_set[problem_id - 1].dim(), 2) + ".csv";
			//Result result(problem_id, NumOfRuns, result_filename);
			/*
			START = clock();
			// Running CCEA in parallel
			omp_set_dynamic(0);			// Explicitly disable dynamic teams
			omp_set_num_threads(4);		// Set num of thread
			#pragma omp parallel
			{
				#pragma omp for 
				for (int run = 0; run < NumOfRuns; run += 1)
				{
					// Set random seed for each run
					time_t t_run = run;
					srand(time(&t_run));
	
					// Setting EA evolve parameters
					Log::set_k((int)run);
					Individual solutions;

					// CC optimize
					//ea->Solve(&solutions, prob_set[problem_id - 1]);
				
					// CC result for each run
					//result[run] = solutions.fitness();
					cout << "Run " << run << ", best fitness: " << solutions.fitness() << endl;
				}
			}
			END = clock();

			result.compute(prob_set[problem_id - 1].global_optimum());
			result.outputToFile(prob_set[problem_id - 1].global_optimum());
			result.close();
			

			cout << "Experiment " << i + 1 << " ended." << " Execution Time: " << (END - START) / CLOCKS_PER_SEC << " sec" << endl << endl;
			*/
			
			
			
		}

		// ini file close
		algo_para_ini.close();
		optimizer_para_ini.close();

		// Destructor
		if (CC != nullptr)
			delete CC;
	}

	allExp_END = clock();

	cout << "All Experiment for is ended." << endl;
	cout << "All Execution Time: " << (allExp_END - allExp_START) / CLOCKS_PER_SEC << " sec" << endl << endl;


	system("pause");
	return 0;
}

