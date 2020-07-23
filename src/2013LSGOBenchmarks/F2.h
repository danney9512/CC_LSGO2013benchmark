#ifndef _F2_H
#define _F2_H

#include "Benchmarks.h"


class F2:public Benchmarks{
protected:

public:
	F2();
	F2(int min_value, int max_value, int dim);
	double compute(double* x) ;
	double compute(const vector<double> &x) ;
	~F2();
};
#endif
