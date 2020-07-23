#ifndef _F10_H
#define _F10_H

#include "Benchmarks.h"

class F10:public Benchmarks
{
protected:
public:
    F10();
    F10(int min_value, int max_value);

    double compute(double* x);
    double compute(const vector<double> &x);

    ~F10();
};

#endif
