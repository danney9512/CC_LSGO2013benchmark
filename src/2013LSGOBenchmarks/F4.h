#ifndef _F4_H
#define _F4_H

#include "Benchmarks.h"

class F4:public Benchmarks
{
protected:
public:
    F4();
    F4(int min_value, int max_value);

    double compute(double* x) ;
    double compute(const vector<double> &x) ;
    /* double compute(vector<double> x) ; */

    ~F4();
};

#endif
