#ifndef _F9_H
#define _F9_H

#include "Benchmarks.h"

class F9:public Benchmarks
{
protected:
public:
    F9();
    F9(int min_value, int max_value);

    double compute(double* x) ;
    double compute(const vector<double> &x) ;

    ~F9();
};

#endif
