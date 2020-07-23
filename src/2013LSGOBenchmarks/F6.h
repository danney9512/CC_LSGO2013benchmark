#ifndef _F6_H
#define _F6_H

#include "Benchmarks.h"

class F6:public Benchmarks
{
protected:
public:
    F6();
    F6(int min_value, int max_value);

    double compute(double* x) ;
    double compute(const vector<double> &x);

    ~F6();
};

#endif
