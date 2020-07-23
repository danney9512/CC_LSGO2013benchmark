#ifndef _F8_H
#define _F8_H

#include "Benchmarks.h"

class F8:public Benchmarks
{
protected:
public:
    F8();
    F8(int min_value, int max_value);

    double compute(double* x) ;
    double compute(const vector<double> &x) ;

    ~F8();
};

#endif
