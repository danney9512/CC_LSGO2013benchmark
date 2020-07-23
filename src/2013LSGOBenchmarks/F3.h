#ifndef _F3_H
#define _F3_H

#include "Benchmarks.h"


class F3:public Benchmarks
{
protected:

public:
    F3();
    F3(int min_value, int max_value, int dim);

    double compute(double* x) ;
    double compute(const vector<double> &x) ;

    ~F3();
};
#endif
