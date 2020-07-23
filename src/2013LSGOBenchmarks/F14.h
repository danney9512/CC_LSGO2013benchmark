#ifndef _F14_H
#define _F14_H

#include "Benchmarks.h"

class F14:public Benchmarks
{
public:
    F14();
    F14(int min_value, int max_value);

    double compute(double* x);
    double compute(const vector<double> &x);

    ~F14();
};

#endif

