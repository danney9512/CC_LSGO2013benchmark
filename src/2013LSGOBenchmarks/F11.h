#ifndef _F11_H
#define _F11_H

#include "Benchmarks.h"

class F11:public Benchmarks
{
protected:
public:
    F11();
    F11(int min_value, int max_value);

    double compute(double* x) ;
    double compute(const vector<double> &x) ;

    ~F11();
};

#endif
