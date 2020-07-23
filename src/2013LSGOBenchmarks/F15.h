#ifndef _F15_H
#define _F15_H

#include "Benchmarks.h"

class F15:public Benchmarks
{
protected:
    //void generateInterArray ( );
public:
    F15();
    F15(int min_value, int max_value);

    double compute(double* x) ;
    double compute(const vector<double> &x);

    ~F15();
};

#endif
