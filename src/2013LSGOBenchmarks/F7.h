#ifndef _F7_H
#define _F7_H

#include "Benchmarks.h"

class F7:public Benchmarks
{
protected:
public:
    F7();
    F7(int min_value, int max_value);

    double compute(double* x) ;
    double compute(const vector<double> &x) ;

    ~F7();
};

#endif
