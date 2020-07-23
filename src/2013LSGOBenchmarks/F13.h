#ifndef _F13_H
#define _F13_H

#include "Benchmarks.h"

class F13:public Benchmarks
{
protected:
public:
    F13();
    F13(int min_value, int max_value);

    double compute(double* x);
    double compute(const vector<double> &x);

    ~F13();
};

#endif

