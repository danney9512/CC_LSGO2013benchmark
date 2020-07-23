#ifndef _F12_H
#define _F12_H

#include "Benchmarks.h"

class F12:public Benchmarks
{
protected:
public:
    F12();
    F12(int min_value, int max_value);

    double compute(double* x);
    double compute(const vector<double> &x);

    ~F12();
};

#endif
