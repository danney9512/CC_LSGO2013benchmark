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

	//BaseEA* ea = nullptr;
	string algo_name, grouping_name, algo_ini_str;
	int problem_id = 0;


	// Compute time initialize
	// 計算時間初始化
	double allExp_START, allExp_END;

	allExp_START = clock();
	// Run Experiment
	for (int i = 0; i < exp_set.size(); i += 1)
	{
		algo_name = exp_set[i].algo_name();
		grouping_name = exp_set[i].grouping();
		problem_id = exp_set[i].problem_id();

		cout << "Experiment" << i + 1 << ", " << algo_name << " solves " << prob_set[problem_id - 1].name() << ": " << endl;
		if (algo_name == "CC")
		{
			// Grouping 
			if (grouping_name == "default")
			{
				// 去讀分群好的檔案不用再分群了
				// 讀檔路徑設定

				// GroupingResult
				// 
			}
			else
			{
				// 執行分群演算法，將分群結果直接給 CC 建構
			}

			// 建構 CC
		}
		else
		{
			cout << "Error msg: Can't recognize the algorithm." << endl;
			continue;
		}

		// Load optimizer 參數
		// Loading Algorithm parameter's setting
		// 載入演算法參數
		algo_ini_str  = "experiments\\" + algo_name + "\\exp_CEC2013benchmark_" + IntToStr(problem_id, 2) + ".ini";
		ifstream exp_algo_para_ini(algo_ini_str);

		if (!exp_algo_para_ini)
		{
			cout << "Error msg: Can't load the CC optimizer's parameters, check the .ini file's path." << endl;
		}
		else
		{
			// Execution the Algorithm
			// 執行演算法

			// Compute time initialize for each experiment
			double START, END;

			// Algorithm parameter's setting
			//ea -> Setup(exp_algo_para_ini);

			string result_filename = algo_name + "_F" + IntToStr(problem_id, 2) + "_D" + IntToStr(prob_set[problem_id - 1].dim(), 2) + ".csv";
			Result result(problem_id, NumOfRuns, result_filename);

			START = clock();
			// Running EA
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
					result[run] = solutions.fitness();
					cout << "Run " << run << ", best fitness: " << solutions[0].fitness() << endl;
				}
			}
			END = clock();

			result.compute(prob_set[problem_id - 1].global_optimum());
			result.outputToFile(prob_set[problem_id - 1].global_optimum());
			result.close();

			cout << "Experiment " << i + 1 << " ended." << " Execution Time: " << (END - START) / CLOCKS_PER_SEC << " sec" << endl << endl;
			
			/*
			if (ea != nullptr) 
				delete ea;
			*/
		}
	}

	allExp_END = clock();

	cout << "All Experiment for is ended." << endl;
	cout << "All Execution Time: " << (allExp_END - allExp_START) / CLOCKS_PER_SEC << " sec" << endl << endl;


	system("pause");
	return 0;
}

