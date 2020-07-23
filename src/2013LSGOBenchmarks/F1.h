#ifndef _F1_H
#define _F1_H

#include "Benchmarks.h"

class F1:public Benchmarks{
public:
	F1();
	F1(int min_value, int max_value, int dim);

	double compute(double* x) ;
	double compute(const vector<double> &x) ;
	~F1();
};
#endif
