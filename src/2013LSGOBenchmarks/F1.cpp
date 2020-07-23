#include "F1.h"

F1::F1():Benchmarks()
{
    Ovector = NULL;
    minX = -100;
    maxX = 100;
    ID = 1;
    anotherz = new double[dimension];
}

F1::F1(int min_value, int max_value, int dim):Benchmarks()
{
    Ovector = NULL;
    minX = min_value;
    maxX = max_value;
    dimension = dim;
    ID = 1;
    anotherz = new double[dimension];
}

F1::~F1()
{
    delete[] Ovector;
    delete[] anotherz;
    // (Ovector);
}


double F1::compute(double* x)
{
    double result;
    int    i;

    if(Ovector == NULL)
    {
        // Ovector = createShiftVector(dimension,minX,maxX);
        Ovector = readOvector();
    }

    for(i = dimension - 1; i >= 0; i--)
    {
        anotherz[i] = x[i] - Ovector[i];
    }

    // T_{OSZ}
    // transform_osz(anotherz,dimension);
    // result = elliptic(anotherz,dimension);
    result = elliptic(anotherz,dimension);
    update(result);
    return(result);
}


double F1::compute(const vector<double> &x)
{
    double result;
    int    i;

    if(Ovector == NULL)
    {
        //Ovector = createShiftVector(dimension,minX,maxX);
        Ovector = readOvector();
    }

    for(i = dimension - 1; i >= 0; i--)
    {
        anotherz[i] = x[i] - Ovector[i];
        //printf("%f\n", anotherz[i]);
    }

    result = elliptic(anotherz,dimension);
    update(result);
    return(result);
}

